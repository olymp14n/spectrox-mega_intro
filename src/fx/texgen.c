#include "texgen.h"
#include <GL/gl.h>
#include "../fw/projection.h"

void fx_texgen_eye_linear_init() {
    fw_projection_perspective();

    float projMatrix[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, projMatrix+0);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_T, GL_EYE_PLANE, projMatrix+4);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_R, GL_EYE_PLANE, projMatrix+8);
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_Q, GL_EYE_PLANE, projMatrix+12);

}

void fx_texgen_eye_linear_begin(float texScale) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glScalef(texScale, -texScale/ fw_projection_getAspect(), 1);

    glMatrixMode(GL_MODELVIEW);
}

void fx_texgen_eye_linear_changeScale(float texScale) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(texScale, -texScale/ fw_projection_getAspect(), 1);

    glMatrixMode(GL_MODELVIEW);
}

void fx_texgen_eye_linear_end() {
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
}
