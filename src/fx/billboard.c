#include "billboard.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "../fw/types.h"
#include "../fw/projection.h"

void fx_billboard_render(fw_object_3d *object3D, fw_image *image) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image->textureID);

    glEnable(GL_DEPTH_TEST);

    // Only draw pixels with alpha 1. Without alpha test transparent billboard pixels would be drawn to the depth buffer.
    // This would result in problems when drawing billboards over each other.
    // https://stackoverflow.com/a/34368825/891846
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 1);

    double projectionMatrix[16];
    double modelViewMatrix[16];
    int viewPort[4];
    double winPos[3];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetIntegerv(GL_VIEWPORT, viewPort);

    fw_projection_ortho();

    glBegin(GL_QUADS);
    for (int i=0; i<object3D->numVertices; i++) {
        gluProject(object3D->v[i].x, object3D->v[i].y, object3D->v[i].z, modelViewMatrix, projectionMatrix, viewPort, &winPos[0], &winPos[1], &winPos[2]);
        winPos[2] *= -1;

        if (winPos[2] < -0.992) {
            glColor3f(.55f,.55f,.65f);
        } else if (winPos[2] < -0.991) {
            glColor3f(.8f,.8f,.8f);
        } else {
            glColor3f(1,1,1);
        }

        fw_sprite_image *sprite = &image->sprites[ object3D->cidx[i] ];
        const int billboardCenter = sprite->width / 2;

        glTexCoord2f(sprite->texTopLeft.x, sprite->texTopLeft.y);
        glVertex3d(winPos[0] - billboardCenter, winPos[1] - billboardCenter, winPos[2]);

        glTexCoord2f(sprite->texTopLeft.x, sprite->texBottomRight.y);
        glVertex3d(winPos[0] - billboardCenter, winPos[1] + billboardCenter, winPos[2]);

        glTexCoord2f(sprite->texBottomRight.x, sprite->texBottomRight.y);
        glVertex3d(winPos[0] + billboardCenter, winPos[1] + billboardCenter, winPos[2]);

        glTexCoord2f(sprite->texBottomRight.x, sprite->texTopLeft.y);
        glVertex3d(winPos[0] + billboardCenter, winPos[1] - billboardCenter, winPos[2]);
    }
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
}
