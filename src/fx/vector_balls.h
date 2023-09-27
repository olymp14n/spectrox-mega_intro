#ifndef FX_VECTOR_BALLS_H
#define FX_VECTOR_BALLS_H

#include "../fw/object_3d.h"

fw_object_3d fx_vector_balls_createCube(int numX, int numY, int numZ, float spacing, int *spriteIndexes, int numSpriteIndexes);
fw_object_3d fx_vector_balls_createSphere(int numSectors, int numStacks, float radius, int *spriteIndexes, int numSpriteIndexes);
fw_object_3d fx_vector_balls_createPyramid(int numY, float spacingXZ, float spacingY, int *spriteIndexes, int numSpriteIndexes);

fw_object_3d fx_vector_balls_createMorphTarget(fw_object_3d *objects, int numObjects, int *spriteIndexes, int numSpriteIndexes);
void fx_vector_balls_morph(fw_object_3d *tgtObj, fw_object_3d *srcObj1, fw_object_3d *srcObj2, float p);


#endif
