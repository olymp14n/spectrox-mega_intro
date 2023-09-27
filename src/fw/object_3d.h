#ifndef FW_OBJECT_3D_H
#define FW_OBJECT_3D_H

#include "types.h"

typedef struct {
    fw_vec3f *v;
    int numVertices;
    fw_vec3i *f;
    int numFaces;
    fw_vec4f *c;
    int *cidx;
    fw_vec3f *fn;
} fw_object_3d;


void fw_object_3d_putFaces(fw_object_3d *object3D);
void fw_object_3d_put(fw_object_3d *object3D);

#endif
