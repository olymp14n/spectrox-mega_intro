#ifndef FW_SOUND_SYNC_H
#define FW_SOUND_SYNC_H

#include "types.h"

typedef struct {
    float pollFreq;
    int (*check_func)(int order, int row, fw_timer_data*);
    void (*execute_proc)(fw_timer_data*);
    int isStarted;
    float deltaTime;
    unsigned int prevRowOrder;
} fw_sound_sync;

void fw_sound_sync_process(fw_sound_sync *sync, fw_timer_data *time);

#endif
