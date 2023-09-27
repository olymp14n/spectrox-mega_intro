#ifndef FW_SOUND_H
#define FW_SOUND_H

#include "hxcmod.h"

void fw_sound_init();
void fw_sound_play();
void fw_sound_shutdown();
tracker_state *fw_sound_getCurrentTrackerState();

#endif
