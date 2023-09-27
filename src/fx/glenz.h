#ifndef FX_GLENZ_H
#define FX_GLENZ_H

#include "../fw/object_3d.h"

void fx_glenz_initCube(fw_object_3d *cube);
void fx_glenz_renderObject(fw_object_3d *object3D);
void fx_glenz_modifyCubeCenterVertices(fw_object_3d *object3D, float scale);

#endif
