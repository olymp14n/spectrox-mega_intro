#include "gradient_border.h"
#include <GL/gl.h>
#include "../config/config.h"
#include "../fw/projection.h"
#include "gradient.h"


void fx_gradient_border_render(fw_timer_data *time, int offsetTop, int offsetBottom) {
    fw_projection_ortho();

    glPushMatrix();
    glLineWidth(1.0);

    fw_vec4f *colors = fx_gradient_getRainbow()->colors;
    int numColors = fx_gradient_getRainbow()->numColors;
    float segLen = FW_RENDER_WIDTH / (float)numColors;

    static float x = 0;
    x -= time->delta*256.f;
    if (x < -FW_RENDER_WIDTH) {
        x += FW_RENDER_WIDTH;
    }
    glTranslatef(x,0,0);
    glBegin(GL_LINES);

    for (int i=0; i<numColors; i++) {
        glColor3f(colors[i].x, colors[i].y, colors[i].z);
        glVertex2f(i * segLen, offsetTop);
        int endIndex = (i+1) % numColors;
        glColor3f(colors[endIndex].x, colors[endIndex].y, colors[endIndex].z);
        glVertex2f(i * segLen + segLen, offsetTop);

        glColor3f(colors[i].x, colors[i].y, colors[i].z);
        glVertex2f(FW_RENDER_WIDTH + i * segLen, offsetTop);
        endIndex = (i+1) % numColors;
        glColor3f(colors[endIndex].x, colors[endIndex].y, colors[endIndex].z);
        glVertex2f(FW_RENDER_WIDTH + i * segLen + segLen, offsetTop);

        glColor3f(colors[i].x, colors[i].y, colors[i].z);
        glVertex2f(i * segLen, FW_RENDER_HEIGHT + offsetBottom);
        glColor3f(colors[endIndex].x, colors[endIndex].y, colors[endIndex].z);
        glVertex2f(i * segLen + segLen, FW_RENDER_HEIGHT + offsetBottom);

        glColor3f(colors[i].x, colors[i].y, colors[i].z);
        glVertex2f(FW_RENDER_WIDTH + i * segLen, FW_RENDER_HEIGHT + offsetBottom);
        glColor3f(colors[endIndex].x, colors[endIndex].y, colors[endIndex].z);
        glVertex2f(FW_RENDER_WIDTH + i * segLen + segLen, FW_RENDER_HEIGHT + offsetBottom);
    }

    glEnd();
    glPopMatrix();
}
