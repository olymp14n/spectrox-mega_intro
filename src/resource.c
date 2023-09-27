#include "resource.h"
#include "fw/image.h"
#include "fw/lut.h"
#include "fw/font.h"
#include "dat/spritesheet_pixels.h"
#include "fx/glenz.h"
#include "fx/hexagon.h"
#include "dat/font_pixels.h"
#include "dat/font_meta.h"
#include "dat/hexagon_pixels_1.h"
#include "dat/hexagon_pixels_2.h"

#define RGBA_BUFFER_WIDTH 320
#define RGBA_BUFFER_HEIGHT 200
#define RGBA_BUFFER_BYTES_PER_PIXEL 4
static fw_image _rgbaBuffer;
static unsigned char _rgbaPixels[RGBA_BUFFER_WIDTH * RGBA_BUFFER_HEIGHT * RGBA_BUFFER_BYTES_PER_PIXEL];

static fw_image _rgbaBuffer2;
static unsigned char _rgbaPixels2[RGBA_BUFFER_WIDTH * RGBA_BUFFER_HEIGHT * RGBA_BUFFER_BYTES_PER_PIXEL];

static fw_object_3d _glenzCubes[1];
static fw_object_3d _hexagons[2];

static fw_image _image;
static fw_sprite_image _spriteImages[8];

static fw_image _hexagonImage2;
static fw_image _hexagonImage1;

static fw_image _defaultFontImage;

void fw_resource_init() {

    fw_lut_init();

    // Image.
    fw_image_initRGBA(&_image, SPRITESHEET_PIXELS_WIDTH, SPRITESHEET_PIXELS_HEIGHT, _spritesheetPixels);
    _image.sprites = _spriteImages;
    _image.numSprites = LEN(_spriteImages);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_MINI_LOGO, 0,0,34,10);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_VEC_BALL_BLUE_NORMAL, 0,32,16,16);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_VEC_BALL_YELLOW_BIG, 32,48,24,24);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_VEC_BALL_YELLOW_SMALL, 32,16,12,12);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_TITLE_LOGO, 71,0,248,122);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_TITLE_GRADIENT, 64,0,1,64);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_GURU_BORDER, 0,122,320,44);
    fw_image_initSpriteImage(&_image, RES_SPRITE_INDEX_GURU_PLAIN, 0,167,320,44);

    // Gradient image.
    fw_image_initRGBA(&_hexagonImage2, HEXAGON_PIXELS_2_WIDTH, HEXAGON_PIXELS_2_HEIGHT, _hexagonPixels2);
    fw_image_initRGBA(&_hexagonImage1, HEXAGON_PIXELS_1_WIDTH, HEXAGON_PIXELS_1_HEIGHT, _hexagonPixels1);

    // Font.
    fw_image_initRGBA(&_defaultFontImage, DEFAULT_FONT_PIXELS_WIDTH, DEFAULT_FONT_PIXELS_HEIGHT, _defaultFontImageData);
    fw_font_initFontFace(&_defaultFontFace, &_defaultFontImage);

    // Empty texture for pixel buffer.
    fw_image_initRGBA(&_rgbaBuffer, RGBA_BUFFER_WIDTH, RGBA_BUFFER_HEIGHT, _rgbaPixels);
    fw_image_initRGBA(&_rgbaBuffer2, RGBA_BUFFER_WIDTH, RGBA_BUFFER_HEIGHT, _rgbaPixels2);

    // Cube.
    fx_glenz_initCube(&_glenzCubes[0]);
    _glenzCubes[0].c[0].x = 0;
    _glenzCubes[0].c[0].y = 1;
    _glenzCubes[0].c[0].z = 0;
    _glenzCubes[0].c[0].w = 1;
    _glenzCubes[0].c[1].x = 0;
    _glenzCubes[0].c[1].y = 1;
    _glenzCubes[0].c[1].z = 0;
    _glenzCubes[0].c[1].w = 1;

    // Hexagons.
    fx_hexagon_init(&_hexagons[0]);
    fx_hexagon_init(&_hexagons[1]);
    fx_hexagon_translateCenterPoint(&_hexagons[0], 0,0,.75);

    for (int i=0; i<_hexagons[0].numFaces; i++) {
        _hexagons[0].c[i].x = .2;
        _hexagons[0].c[i].y = 0;
        _hexagons[0].c[i].z = .5f + i * .5f/(_hexagons[0].numFaces-1);
    }

    for (int i=0; i<_hexagons[1].numFaces; i++) {
        _hexagons[1].c[i].x = 1;
        _hexagons[1].c[i].y = 1;
        _hexagons[1].c[i].z = 1;
    }
}


fw_image *getImage() {
    return &_image;
}

fw_object_3d *getGlenzCubes() {
    return _glenzCubes;
}

fw_object_3d *getHexagons() {
    return _hexagons;
}

fw_image *getRGBABuffer() {
    return &_rgbaBuffer;
}

fw_image *getRGBABuffer2() {
    return &_rgbaBuffer2;
}

fw_font_face *getDefaultFontFace() {
    return &_defaultFontFace;
}

fw_image *getHexagonImage2() {
    return &_hexagonImage2;
}

fw_image *getHexagonImage1() {
    return &_hexagonImage1;
}
