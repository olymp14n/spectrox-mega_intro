#include "gradient.h"


static fw_vec4f _rainbowColors[] = {
    { 1.0f, 0.0f, 0.0f },
    { 1.0f, 0.5f, 0.0f },
    { 1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 0.4f, 0.0f, 0.6f },
};

static fx_gradient _rainbowGradient = {
    .colors = _rainbowColors,
    .numColors = LEN(_rainbowColors),
};


fx_gradient *fx_gradient_getRainbow() {
    return &_rainbowGradient;
}


void fx_gradient_spread(fx_gradient *source, fx_gradient *target, int isLoopEnabled) {
    const fw_vec4f *gradientKeys = source->colors;
    const int numGradientKeys = source->numColors;

    int loopGradientOffset = -1;
    if (isLoopEnabled) {
        loopGradientOffset = 0;
    }

    fw_vec4f *colors = target->colors;
    const int numColors = target->numColors;
    for (int i=0; i<numColors; i++) {
        float gradientPos = (numGradientKeys + loopGradientOffset) * ((float)i)/(numColors-1);
        float highFrac = gradientPos - ((int)gradientPos);
        float lowFrac = 1.f - highFrac;
        int lowIdx = (int)gradientPos;
        int highIdx = (lowIdx + 1) % numGradientKeys;

        colors[i].x = lowFrac * gradientKeys[lowIdx].x + highFrac * gradientKeys[highIdx].x;
        colors[i].y = lowFrac * gradientKeys[lowIdx].y + highFrac * gradientKeys[highIdx].y;
        colors[i].z = lowFrac * gradientKeys[lowIdx].z + highFrac * gradientKeys[highIdx].z;
    }

}
