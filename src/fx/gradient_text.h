#ifndef FX_GRADIENT_TEXT_H
#define FX_GRADIENT_TEXT_H

#include "../fw/types.h"
#include "../fw/image.h"
#include "../fw/font.h"
#include "gradient.h"

typedef struct {
    fw_image *target;
    unsigned char *text;
    int textLength;
    fw_font_face *fontFace;
    fx_gradient *gradient;
    int gradientOffset;
    int outputXMin;
    int outputXMax;
    float fontSize;
    float xStart;
    float yStart;
    float xSineAmplitude;
    float xSineFrequency;
    float xSineCharacterOffsetFactor;
    float ySineAmplitude;
    float ySineFrequency;
    float ySineCharacterOffsetFactor;

} fx_gradient_text_properties;

/**
 * NOTE: `fw_lut` must be initialized before calling this procedure.
 */
void fx_gradient_text_write(fx_gradient_text_properties *props, float timeElapsed);

void fx_gradient_text_writeBegin(fw_image *targetImage);
void fx_gradient_text_writeEnd(fw_image *targetImage);

#endif
