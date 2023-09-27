#ifndef FX_STARFIELD_2D_H
#define FX_STARFIELD_2D_H

#include "../fw/types.h"

void fx_starfield_2d_init();

/**
 * NOTE: `fw_lut` must be initialized before calling this procedure.
 */
void fx_starfield_2d_render(fw_timer_data *time);

#endif
