#include "object_3d.h"
#include <stdlib.h>
#include <GL/gl.h>

void fw_object_3d_putFaces(fw_object_3d *object3D) {
    fw_vec3f *v = object3D->v;
    fw_vec3i *f = object3D->f;
    fw_vec4f *c = object3D->c;
    int *cidx = object3D->cidx;
    fw_vec3f *fn = object3D->fn;

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < object3D->numFaces; i++) {
        glColor4f(c[cidx[i]].x, c[cidx[i]].y, c[cidx[i]].z, c[cidx[i]].w);
        glNormal3f(fn[i].x, fn[i].y, fn[i].z);
        glVertex3f(v[f[i].x].x, v[f[i].x].y, v[f[i].x].z);
        glVertex3f(v[f[i].y].x, v[f[i].y].y, v[f[i].y].z);
        glVertex3f(v[f[i].z].x, v[f[i].z].y, v[f[i].z].z);
    }
    glEnd();
}

void fw_object_3d_put(fw_object_3d *object3D) {
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    fw_object_3d_putFaces(object3D);
    glPopAttrib();
}
