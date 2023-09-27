#include "sound_sync.h"
#include "sound.h"

void fw_sound_sync_process(fw_sound_sync *sync, fw_timer_data *time) {
    sync->deltaTime = sync->deltaTime + time->delta;
    if (sync->deltaTime > sync->pollFreq) {
        sync->deltaTime = 0;

        tracker_state *ts = fw_sound_getCurrentTrackerState();
        unsigned int row = ts->cur_pattern_pos;
        unsigned int order = ts->cur_pattern_table_pos;

        unsigned int rowOrder = order << 16 | row;
        if (!sync->isStarted || sync->prevRowOrder != rowOrder) {
            sync->isStarted = 1;
            sync->prevRowOrder = rowOrder;

            if (sync->check_func(order, row, time)) {
                sync->execute_proc(time);
            }
        }
    }
}
