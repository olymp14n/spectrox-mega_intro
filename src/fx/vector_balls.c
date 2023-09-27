#include "vector_balls.h"

#include <stdlib.h>
#include <math.h>
#include "../fw/types.h"

fw_object_3d fx_vector_balls_createCube(int numX, int numY, int numZ, float spacing, int *spriteIndexes, int numSpriteIndexes) {
    const int numVertices = numX * numY * numZ;
    fw_vec3f *v = calloc(numVertices, sizeof(fw_vec3f));
    int *cidx = calloc(numVertices, sizeof(int));

    int i = 0;
    for (int y=0; y<numY; y++) {
        for (int z=0; z<numZ; z++) {
            for (int x=0; x<numX; x++) {
                v[i].x = spacing * (x - .5 * (numX-1));
                v[i].y = spacing * (y - .5 * (numY-1));
                v[i].z = spacing * (z - .5 * (numZ-1));
                cidx[i] = spriteIndexes[i % numSpriteIndexes];
                i++;
            }
        }
    }

    fw_object_3d object3d = {
        .v = v,
        .numVertices = numVertices,
        .cidx = cidx
    };
    return object3d;
}


fw_object_3d fx_vector_balls_createSphere(int numSectors, int numStacks, float radius, int *spriteIndexes, int numSpriteIndexes) {
    // From http://www.songho.ca/opengl/gl_sphere.html

    if (numSectors < 1) {
        numSectors = 1;
    }
    if (numStacks < 3) {
        numStacks = 3;
    }

    int numVertices = (numSectors) * (numStacks+1);
    numVertices -= 2 * (numSectors-1); // Adjustment: top and bottom have only one vertex.
    numVertices -= 2 * (int)(.5f + .5f * numSectors); // Adjustment: first and last ring have only half the vertices (rounded).

    fw_vec3f *v = calloc(numVertices, sizeof(fw_vec3f));
    int *cidx = calloc(numVertices, sizeof(int));

    const float sectorStep = 2 * M_PI / numSectors;
    const float stackStep = M_PI / numStacks;

    int i = 0;
    for(int y = 0; y < numStacks+1; y++) {
        float stackAngle = M_PI / 2 - y * stackStep;
        for(int x = 0; x < numSectors; x++) {
            // Top and bottom row: only place one point.
            if ( (y == 0 && x != 0) || (y == numStacks && x != numSectors-1) ) {
                continue;
            }

            // First and last ring have only half the vertices.
            if ( (y==1 && x%2 == 0) || (y == numStacks-1 && x%2 == 0) ) {
                continue;
            }

            float sectorAngle = x * sectorStep;
            v[i].x = radius * cosf(stackAngle) * cosf(sectorAngle);
            v[i].y = radius * cosf(stackAngle) * sinf(sectorAngle);
            v[i].z = radius * sinf(stackAngle);
            cidx[i] = spriteIndexes[i % numSpriteIndexes];
            i++;
        }
    }

    fw_object_3d object3d = {
        .v = v,
        .numVertices = numVertices,
        .cidx = cidx
    };
    return object3d;
}


fw_object_3d fx_vector_balls_createPyramid(int numY, float spacingXZ, float spacingY, int *spriteIndexes, int numSpriteIndexes) {
    // Count vertices.
    int i=0;
    for (int y=0; y<numY; y++) {
        const int numX = y+1;
        const int numZ = y+1;
        for (int x=0; x<numX; x++) {
            for  (int z=0; z<numZ; z++) {
                i++;
            }
        }
    }

    const int numVertices = i;
    fw_vec3f *v = calloc(numVertices, sizeof(fw_vec3f));
    int *cidx = calloc(numVertices, sizeof(int));

    i=0;
    for (int y=0; y<numY; y++) {
        const int numX = y+1;
        const int numZ = y+1;
        for (int x=0; x<numX; x++) {
            for  (int z=0; z<numZ; z++) {
                v[i].x = spacingXZ * (x - .5 * (numX-1));
                v[i].y = spacingY * (y - .5 * numY);

                // Render from back to front to give better color pattern.
                if (y%2 != 0 && x%2 != 0) {
                    v[i].z = spacingXZ * (z - numZ/2.f);
                } else {
                    v[i].z = spacingXZ * ((numZ-1-z) - numZ/2.f);
                }

                cidx[i] = spriteIndexes[i % numSpriteIndexes];
                i++;
            }
        }
    }

    fw_object_3d object3d = {
        .v = v,
        .numVertices = numVertices,
        .cidx = cidx
    };
    return object3d;
}


fw_object_3d fx_vector_balls_createMorphTarget(fw_object_3d *objects, int numObjects, int *spriteIndexes, int numSpriteIndexes) {

    int maxNumVertices = 0;
    for (int i=0; i<numObjects; i++) {
        if (objects[i].numVertices > maxNumVertices) {
            maxNumVertices = objects[i].numVertices;
        }
    }

    fw_vec3f *v = calloc(maxNumVertices, sizeof(fw_vec3f));
    int *cidx = calloc(maxNumVertices, sizeof(int));


    for (int i=0; i<maxNumVertices; i++) {
        cidx[i] = spriteIndexes[i % numSpriteIndexes];
    }

    fw_object_3d object3d = {
        .v = v,
        .numVertices = maxNumVertices,
        .cidx = cidx
    };

    fx_vector_balls_morph(&object3d, &objects[0], &objects[0], 0);

    return object3d;
}

/*
 * Morph between to vector balls objects. `tgtObj` must be prepared to contain the same amount of vertices as the bigger source object
 * and must contain sprite indexes for exactly two sprites.
 */
void fx_vector_balls_morph(fw_object_3d *tgtObj, fw_object_3d *srcObj1, fw_object_3d *srcObj2, float p) {
    const int evenNumVertices1 = (srcObj1->numVertices % 2 == 0) ? srcObj1->numVertices : srcObj1->numVertices - 1;
    const int evenNumVertices2 = (srcObj2->numVertices % 2 == 0) ? srcObj2->numVertices : srcObj2->numVertices - 1;

    for (int i=0; i<tgtObj->numVertices; i++) {
        int src1Idx = (i >= srcObj1->numVertices) ? i%evenNumVertices1 : i;
        int src2Idx = (i >= srcObj2->numVertices) ? i%evenNumVertices2 : i;

        tgtObj->v[i].x = (1.f - p) * srcObj1->v[src1Idx].x + p * srcObj2->v[src2Idx].x;
        tgtObj->v[i].y = (1.f - p) * srcObj1->v[src1Idx].y + p * srcObj2->v[src2Idx].y;
        tgtObj->v[i].z = (1.f - p) * srcObj1->v[src1Idx].z + p * srcObj2->v[src2Idx].z;
    }
}
