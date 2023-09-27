#ifndef FW_IMAGE_H
#define FW_IMAGE_H

#include <GL/gl.h>
#include "types.h"

typedef struct {
    int width;
    int height;
    fw_vec2i topLeft;
    fw_vec2i bottomRight;
    fw_vec2f texTopLeft;
    fw_vec2f texBottomRight;
} fw_sprite_image;

typedef struct {
    int width;
    int height;
    int bytesPerPixel;
    GLuint textureID;
    unsigned char *pixels;
    int numSprites;
    fw_sprite_image *sprites;
} fw_image;

void fw_image_initRGBA(fw_image *image, int width, int height, unsigned char *pixels);
void fw_image_clear(fw_image *image);

void fw_image_initSpriteImage(fw_image *image, int spriteIndex, int x, int y, int width, int height);
void fw_image_putSpriteImage(fw_sprite_image *sprite);

void fw_image_putImage(fw_image *image);

void fw_image_renderBegin(fw_image *image, float alpha);
void fw_image_renderEnd();

#endif
