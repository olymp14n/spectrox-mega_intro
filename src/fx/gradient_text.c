#include "gradient_text.h"
#include <GL/gl.h>
#include "../config/config.h"
#include "../fw/lut.h"

void fx_gradient_text_write(fx_gradient_text_properties *props, float timeElapsed) {
    fw_image *target = props->target;
    unsigned char *text = props->text;
    int textLength = props->textLength;
    fw_font_face *fontFace = props->fontFace;
    fw_image *fontImage = props->fontFace->image;
    fx_gradient *gradient = props->gradient;
    int gradientOffset = props->gradientOffset;
    int outputXMin = props->outputXMin;
    int outputXMax = props->outputXMax > 0 ? props->outputXMax : FW_RENDER_WIDTH;
    float fontSize = props->fontSize > 0 ? props->fontSize : 1;
    float xStart = props->xStart;
    float yStart = props->yStart;
    float xSineAmplitude = props->xSineAmplitude;
    float xSineFrequency = props->xSineFrequency;
    float xSineCharacterOffsetFactor = props->xSineCharacterOffsetFactor;
    float ySineAmplitude = props->ySineAmplitude;
    float ySineFrequency = props->ySineFrequency;
    float ySineCharacterOffsetFactor = props->ySineCharacterOffsetFactor;

    unsigned char *rgba = target->pixels;
    fw_font_glyph *glyph;

    int xadv = 0;

    for (int i=0; i<textLength; i++) {
        glyph = &fontFace->glyphs[text[i]];

        int glyphPosX = xStart + xadv * fontSize;
        if (glyphPosX < outputXMin - glyph->xadvance * fontSize || glyphPosX > outputXMax) {
            xadv += glyph->xadvance;
            continue;
        }

        const float yAdd = ySineAmplitude * fw_lut_sin(i*ySineCharacterOffsetFactor  + timeElapsed*ySineFrequency);
        const float xAdd = xSineAmplitude * fw_lut_sin(i*xSineCharacterOffsetFactor  + timeElapsed*xSineFrequency);

        for (int y=0; y<glyph->height; y++) {
            const float rgbaY = (int)(yStart + fontSize * (y + glyph->yoffset) + yAdd + .5f);
            for (int x=0; x<glyph->width; x++) {
                int fontIdx = fontImage->bytesPerPixel * ((x + glyph->x) + (y + glyph->y) * fontImage->width);
                if (fontImage->pixels[fontIdx] > 0) {
                    const float rgbaX = (int)(xStart + fontSize*(x + glyph->xoffset + xadv) + xAdd + .5f);
                    if (rgbaX <= 0 || rgbaX >= target->width || rgbaY <= 0 || rgbaY >= target->height) {
                        continue;
                    }

                    const int rgbaIdx = target->bytesPerPixel * (rgbaX + rgbaY * target->width);
                    const int gradIdx = (int)(rgbaY + gradientOffset) % (gradient->numColors-1);
                    rgba[rgbaIdx] = gradient->colors[gradIdx].x * 255;
                    rgba[rgbaIdx+1] = gradient->colors[gradIdx].y * 255;
                    rgba[rgbaIdx+2] = gradient->colors[gradIdx].z * 255;
                    rgba[rgbaIdx+3] = 255;
                }
            }
        }
        xadv += glyph->xadvance;
    }
}

void fx_gradient_text_writeBegin(fw_image *targetImage) {
    fw_image_clear(targetImage);
}

void fx_gradient_text_writeEnd(fw_image *targetImage) {
    fw_image_renderBegin(targetImage, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, targetImage->width, targetImage->height, GL_RGBA, GL_UNSIGNED_BYTE, targetImage->pixels);
    fw_image_putImage(targetImage);
    fw_image_renderEnd();
}


