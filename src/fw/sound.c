#include "sound.h"
#include <windows.h>
#include "../dat/song_data.h"

#define SAMPLERATE 44100
#define NUM_STEREO_16_BIT_SAMPLES 16384
#define SOUND_BUFFER_SIZE (NUM_STEREO_16_BIT_SAMPLES * 2)
#define NUM_TRACKER_STATES 100

static msample _soundBuffer1[SOUND_BUFFER_SIZE];
static msample _soundBuffer2[SOUND_BUFFER_SIZE];
static WAVEHDR _waveHeader1;
static WAVEHDR _waveHeader2;
static HWAVEOUT _waveOutHandle;

static BOOL _isReady = FALSE;
static DWORD _threadID;

static modcontext _modContext;
static tracker_state _trackerState1[NUM_TRACKER_STATES];
static tracker_buffer_state _trackerBufferState1;
static tracker_state _trackerState2[NUM_TRACKER_STATES];
static tracker_buffer_state _trackerBufferState2;

static void fw_sound_threadProc() {
    _isReady = TRUE;
    MSG msg;
    while (_isReady && GetMessage(&msg, 0, 0, 0)) {

        switch (msg.message) {
        case MM_WOM_OPEN:
            break;
        case MM_WOM_CLOSE:
            _isReady = FALSE;
            break;
        case MM_WOM_DONE: {
            WAVEHDR *wh =  (WAVEHDR*)msg.lParam;

            // Determine the tracker state buffer belonging to the sound buffer that is being filled.
            tracker_buffer_state *tbs;
            if (wh->lpData == (LPSTR) & _soundBuffer1) {
                tbs = &_trackerBufferState1;
            } else {
                tbs = &_trackerBufferState2;
            }

            tbs->nb_of_state = 0;
            hxcmod_fillbuffer(&_modContext, (msample*)wh->lpData, NUM_STEREO_16_BIT_SAMPLES, tbs);
            waveOutWrite(_waveOutHandle, wh, wh->dwBufferLength);
            break;
        }
        default:
            break;
        }
    }
}

static void fw_sound_initTrackerStateBuffer(tracker_buffer_state *trackerBufferState, tracker_state *trackerState) {
    trackerBufferState->nb_of_state = 0;
    trackerBufferState->nb_max_of_state = NUM_TRACKER_STATES;
    trackerBufferState->track_state_buf = trackerState;
    trackerBufferState->sample_step = NUM_STEREO_16_BIT_SAMPLES / trackerBufferState->nb_max_of_state;
}

static void fw_sound_initWaveFormat(WAVEFORMATEX *waveFormat) {
    waveFormat->wFormatTag = WAVE_FORMAT_PCM;
    waveFormat->nChannels = 2;
    waveFormat->nSamplesPerSec = SAMPLERATE;
    waveFormat->nAvgBytesPerSec = waveFormat->nSamplesPerSec * 4;
    waveFormat->nBlockAlign = 4;
    waveFormat->wBitsPerSample = 16;
    waveFormat->cbSize = 0;
}

static void fw_sound_initWaveHeader(WAVEHDR *waveHeader, char *soundBuffer, DWORD soundBufferSize) {
    waveHeader->lpData=soundBuffer;
    waveHeader->dwBufferLength=soundBufferSize;
    waveHeader->dwFlags=0;
    waveHeader->dwLoops=0;
}

void fw_sound_init() {
    HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) fw_sound_threadProc, NULL, 0, &_threadID);
    CloseHandle(threadHandle);

    hxcmod_init(&_modContext);

    if (!hxcmod_setcfg(&_modContext, SAMPLERATE, 1, 1)) {
        exit(1);
    }

    if (!hxcmod_load(&_modContext, _songData, sizeof(_songData))) {
        exit(2);
    }

    fw_sound_initTrackerStateBuffer(&_trackerBufferState1, _trackerState1);
    hxcmod_fillbuffer(&_modContext,_soundBuffer1, NUM_STEREO_16_BIT_SAMPLES, &_trackerBufferState1);

    fw_sound_initTrackerStateBuffer(&_trackerBufferState2, _trackerState2);
    hxcmod_fillbuffer(&_modContext,_soundBuffer2, NUM_STEREO_16_BIT_SAMPLES, &_trackerBufferState2);

    WAVEFORMATEX waveFormat = {0};
    fw_sound_initWaveFormat(&waveFormat);

    // Open audio device and start passing messages to the callback thread.
    if (waveOutOpen(&_waveOutHandle, WAVE_MAPPER, &waveFormat, _threadID, 0, CALLBACK_THREAD) != MMSYSERR_NOERROR) {
        exit(3);
    }

    fw_sound_initWaveHeader(&_waveHeader1, (char*)_soundBuffer1, sizeof(_soundBuffer1));
    fw_sound_initWaveHeader(&_waveHeader2, (char*)_soundBuffer2, sizeof(_soundBuffer2));

    if (waveOutPrepareHeader(_waveOutHandle, &_waveHeader1, sizeof(_waveHeader1)) != MMSYSERR_NOERROR) {
        waveOutClose(_waveOutHandle);
        exit(4);
    }

    if (waveOutPrepareHeader(_waveOutHandle, &_waveHeader2, sizeof(_waveHeader2)) != MMSYSERR_NOERROR) {
        waveOutClose(_waveOutHandle);
        exit(5);
    }
}

void fw_sound_play() {
    while (!_isReady) {
        Sleep(100);
    }
    waveOutWrite(_waveOutHandle, &_waveHeader1, sizeof(_waveHeader1));
    waveOutWrite(_waveOutHandle, &_waveHeader2, sizeof(_waveHeader2));
}

void fw_sound_shutdown() {
    waveOutReset(_waveOutHandle);
    waveOutUnprepareHeader(_waveOutHandle, &_waveHeader1, sizeof(_waveHeader1));
    waveOutUnprepareHeader(_waveOutHandle, &_waveHeader2, sizeof(_waveHeader2));
    waveOutClose(_waveOutHandle);
}

static int fw_sound_getTrackerBufferIndex(tracker_buffer_state *trackerBufferState, int numTrackerStates, DWORD searchValue) {
    // Binary search for the tracker state that has the nearest value to a searchValue.
    tracker_state *trackerStates = trackerBufferState->track_state_buf;

    int low = 0;
    int high = numTrackerStates - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (trackerStates[mid].buf_index == searchValue) {
            return mid;
        } else if (trackerStates[mid].buf_index < searchValue) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // If we get here, low and high values have crossed. Variable `high` now contains the lower index.
    // Return the index of the lower value, i.e. index stored in `high`.
    return high;

    // Variant to return the index with the nearest value.
    // return abs(trackerStates[low].buf_index - searchValue) < abs(trackerStates[high].buf_index - searchValue) ? low : high;
}

tracker_state *fw_sound_getCurrentTrackerState() {
    MMTIME mmt;
    mmt.wType = TIME_SAMPLES;

    if (waveOutGetPosition(_waveOutHandle, &mmt, sizeof(MMTIME)) != MMSYSERR_NOERROR) {
        exit(6);
    }

    // Calculate sample number offset by masking the current total sample number (i.e. calculate the modulus).
    DWORD sampleOffset = mmt.u.sample & (NUM_STEREO_16_BIT_SAMPLES - 1);

    // Determine if our first or second buffer is being played.
    tracker_buffer_state *trackerBufferState;
    if (mmt.u.sample & NUM_STEREO_16_BIT_SAMPLES) {
        trackerBufferState = &_trackerBufferState2;
    } else {
        trackerBufferState = &_trackerBufferState1;
    }

    int i = fw_sound_getTrackerBufferIndex(trackerBufferState, trackerBufferState->nb_of_state, sampleOffset);
    return &trackerBufferState->track_state_buf[i];
}

