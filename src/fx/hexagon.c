#include "hexagon.h"
#include <stdlib.h>
#include <string.h>
#include "../fw/types.h"

// Vertices.
static fw_vec3f _v[] = {
    {0, -1, 0}, // 0=Bottom.
    {-0.866025, -0.5, 0},
    {-0.866025, 0.5, 0},
    {0, 1, 0},
    {0.866025, .5, 0},
    {0.866025, -.5, 0},
    {0, 0, 0}, // 6=Center.
};

// Faces.
static fw_vec3i _f[] = {
    {0, 5, 6},
    {5, 4, 6},
    {4, 3, 6},
    {3, 2, 6},
    {2, 1, 6},
    {1, 0, 6},
};

// Colors.
static fw_vec4f _c[] = {
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {1, 0, 0, 1},
    {1, 1, 1, 1},
    {1, 0, 0, 1},
};

// Color indexes.
static int _cidx[] = {
    0,1,2,3,4,5
};

// Normals.
static fw_vec3f _fn[] = {
    {0, 0, -1},
    {0, 0, -1},
    {0, 0, -1},
    {0, 0, -1},
    {0, 0, -1},
    {0, 0, -1},
};

void fx_hexagon_init(fw_object_3d *object3D) {
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

void fx_hexagon_translateCenterPoint(fw_object_3d *object3D, float x, float y, float z) {
    object3D->v[6].x += x;
    object3D->v[6].y += y;
    object3D->v[6].z += z;
}
