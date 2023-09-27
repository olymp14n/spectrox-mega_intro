#ifndef FW_FONT_H
#define FW_FONT_H

#include <GL/gl.h>
#include "types.h"
#include "image.h"

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
    float top;
    float left;
    float bottom;
    float right;
} fw_font_glyph;

typedef struct {
    int lineHeight;
    int textureWidth;
    int textureHeight;
    fw_font_glyph *glyphs;
    int numGlyphs;
    fw_image *image;
} fw_font_face;

void fw_font_initFontFace(fw_font_face *fontFace, fw_image *image);
void fw_font_renderText(char *strings[], int numLines, fw_font_face *fontFace, int x, int y, float completion);

#endif
