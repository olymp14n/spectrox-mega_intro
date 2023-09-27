#ifndef FX_GRADIENT_H
#define FX_GRADIENT_H

#include "../fw/types.h"

typedef struct {
    fw_vec4f *colors;
    int numColors;
} fx_gradient;

fx_gradient *fx_gradient_getRainbow();
void fx_gradient_spread(fx_gradient *source, fx_gradient *target, int isLoopEnabled);


#endif
