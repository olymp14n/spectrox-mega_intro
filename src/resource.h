#ifndef FW_RESOURCE_H
#define FW_RESOURCE_H

#include <GL/gl.h>
#include "fw/font.h"
#include "fw/object_3d.h"
#include "fw/image.h"

#define RES_SPRITE_INDEX_MINI_LOGO 0
#define RES_SPRITE_INDEX_VEC_BALL_BLUE_NORMAL 1
#define RES_SPRITE_INDEX_VEC_BALL_YELLOW_BIG 2
#define RES_SPRITE_INDEX_VEC_BALL_YELLOW_SMALL 3
#define RES_SPRITE_INDEX_TITLE_LOGO 4
#define RES_SPRITE_INDEX_TITLE_GRADIENT 5
#define RES_SPRITE_INDEX_GURU_BORDER 6
#define RES_SPRITE_INDEX_GURU_PLAIN 7

void fw_resource_init();

fw_image *getImage();
fw_object_3d *getGlenzCubes();
fw_object_3d *getHexagons();
fw_image *getRGBABuffer();
fw_image *getRGBABuffer2();
fw_font_face *getDefaultFontFace();
fw_image *getHexagonImage2();
fw_image *getHexagonImage1();

#endif
