#include "image.h"
#include <GL/glu.h>
#include "../config/config.h"
#include "projection.h"

void fw_image_initRGBA(fw_image *image, int width, int height, unsigned char *pixels) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    image->width = width;
    image->height = height;
    image->bytesPerPixel = 4;
    image->textureID = textureID;
    image->pixels = pixels;
}

void fw_image_clear(fw_image *image) {
    ZeroMemory(image->pixels, image->width * image->height * image->bytesPerPixel);
}

void fw_image_initSpriteImage(fw_image *image, int spriteIndex, int x, int y, int width, int height) {
    fw_sprite_image *sprite = &image->sprites[spriteIndex];
    sprite->width = width;
    sprite->height = height;
    sprite->topLeft.x = x;
    sprite->topLeft.y = y;
    sprite->bottomRight.x = x + width;
    sprite->bottomRight.y = y + height;
    sprite->texTopLeft.x = x / (float)image->width;
    sprite->texTopLeft.y = y / (float)image->height;
    sprite->texBottomRight.x = (x + width)/(float)image->width;
    sprite->texBottomRight.y = (y + height)/(float)image->height;
}

void fw_image_putSpriteImage(fw_sprite_image *sprite) {
    glBegin(GL_QUADS);

    glTexCoord2f(sprite->texTopLeft.x, sprite->texTopLeft.y);
    glVertex2i(0, 0);

    glTexCoord2f(sprite->texTopLeft.x, sprite->texBottomRight.y);
    glVertex2i(0, sprite->height);

    glTexCoord2f(sprite->texBottomRight.x, sprite->texBottomRight.y);
    glVertex2i(sprite->width, sprite->height);

    glTexCoord2f(sprite->texBottomRight.x, sprite->texTopLeft.y);
    glVertex2i(sprite->width, 0);

    glEnd();
}

void fw_image_putImage(fw_image *image) {
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2i(0, 0);

    glTexCoord2d(0, 1);
    glVertex2i(0, image->height);

    glTexCoord2d(1, 1);
    glVertex2i(image->width, image->height);

    glTexCoord2d(1, 0);
    glVertex2i(image->width, 0);
    glEnd();
}

void fw_image_renderBegin(fw_image *image, float alpha) {
    glColor4f(1,1,1, alpha);
    fw_projection_ortho();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image->textureID);
    glPushMatrix();
}

void fw_image_renderEnd() {
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glClear(GL_DEPTH_BUFFER_BIT);
}

