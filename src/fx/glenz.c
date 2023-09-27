#include "glenz.h"
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include "../fw/object_3d.h"

// vertices
static fw_vec3f _v[] = {
    // front
    {-1, -1,  1}, // 0
    { 1, -1,  1}, // 1
    { 1,  1,  1}, // 2
    {-1,  1,  1}, // 3
    // back
    {-1, -1, -1}, // 4
    { 1, -1, -1}, // 5
    { 1,  1, -1}, // 6
    {-1,  1, -1}, // 7
    // mid
    { 0,  0,  1}, // 8  front
    { 0,  0, -1}, // 9  back
    { 0,  1,  0}, // 10 top
    { 0, -1,  0}, // 11 bottom
    { 1,  0,  0}, // 12 right
    {-1,  0,  0}, // 13 left
};

// faces
static fw_vec3i _f[] = {
    // front
    {0, 1, 8},
    {3, 8, 2},
    {3, 0, 8},
    {8, 1, 2},
    // back
    {7, 6, 9},
    {4, 9, 5},
    {7, 9, 4},
    {9, 6, 5},
    // right
    {1, 5, 12},
    {2, 12, 6},
    {2, 1, 12},
    {12, 5, 6},
    // left
    {0, 13, 4},
    {3, 7, 13},
    {3, 13, 0},
    {13, 7, 4},
    // top
    {3, 2, 10},
    {7, 10, 6},
    {7, 3, 10},
    {10, 2, 6},
    // bottom
    {0, 11, 1},
    {4, 5, 11},
    {4, 11, 0},
    {11, 5, 1},
};

// colors
static fw_vec4f _c[] = {
    {1, 1, 1, 1.f},
    {1, 0, 0, .6f},
};

// color index
static int _cidx[] = {
    // front
    0,0,1,1,
    // back
    0,0,1,1,
    // right
    1,1,0,0,
    // left
    1,1,0,0,
    // top
    1,1,0,0,
    // bottom
    1,1,0,0,
};

// face normals
static fw_vec3f _fn[] = {
    // front
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    // back
    {0, 0, -1},
    {0, 0, -1},
    {0, 0, -1},
    {0, 0, -1},
    // right
    {1, 0, 0},
    {1, 0, 0},
    {1, 0, 0},
    {1, 0, 0},
    // left
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    // top
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    // bottom
    {0, -1, 0},
    {0, -1, 0},
    {0, -1, 0},
    {0, -1, 0},
};


void fx_glenz_initCube(fw_object_3d *object3D) {
    object3D->v = calloc(LEN(_v), sizeof(_v[0]));
    memcpy(object3D->v, _v, sizeof(_v));
    object3D->numVertices = LEN(_v);

    object3D->f = calloc(LEN(_f), sizeof(_f[0]));
    memcpy(object3D->f, _f, sizeof(_f));
    object3D->numFaces = LEN(_f);

    object3D->c = calloc(LEN(_c), sizeof(_c[0]));
    memcpy(object3D->c, _c, sizeof(_c));

    object3D->cidx = calloc(LEN(_cidx), sizeof(_cidx[0]));
    memcpy(object3D->cidx, _cidx, sizeof(_cidx));

    object3D->fn = calloc(LEN(_fn), sizeof(_fn[0]));
    memcpy(object3D->fn, _fn, sizeof(_fn));
}

void fx_glenz_renderObject(fw_object_3d *object3D) {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    fw_object_3d_putFaces(object3D);
    glCullFace(GL_BACK);
    fw_object_3d_putFaces(object3D);
    glPopAttrib();
}

void fx_glenz_modifyCubeCenterVertices(fw_object_3d *object3D, float scale) {
    object3D->v[8].z = scale;
    object3D->v[9].z = -scale;
    object3D->v[10].y = scale;
    object3D->v[11].y = -scale;
    object3D->v[12].x = scale;
    object3D->v[13].x = -scale;
}

