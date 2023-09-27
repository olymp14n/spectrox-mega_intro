#include "starfield_2d.h"
#include <GL/gl.h>
#include "../config/config.h"
#include "../fw/projection.h"
#include "../fw/lut.h"


fw_vec2f _starsBack[40];
fw_vec2f _starsMiddle[20];
fw_vec2f _starsFront[20];

void fx_starfield_2d_init() {
    srand(1);

    for (int i=0; i<LEN(_starsBack); i++) {
        _starsBack[i].x = FW_RENDER_WIDTH * ((float)rand() / RAND_MAX);
        _starsBack[i].y = FW_RENDER_HEIGHT * ((float)rand() / RAND_MAX);
    }

    for (int i=0; i<LEN(_starsMiddle); i++) {
        _starsMiddle[i].x = FW_RENDER_WIDTH * ((float)rand() / RAND_MAX);
        _starsMiddle[i].y = FW_RENDER_HEIGHT * ((float)rand() / RAND_MAX);
    }

    for (int i=0; i<LEN(_starsFront); i++) {
        _starsFront[i].x = FW_RENDER_WIDTH * ((float)rand() / RAND_MAX);
        _starsFront[i].y = FW_RENDER_HEIGHT * ((float)rand() / RAND_MAX);
    }

}

void fx_starfield_2d_render(fw_timer_data *time) {
    glDisable(GL_DEPTH_TEST);

    fw_projection_ortho();

    glPushMatrix();
    glPointSize(1);

    glBegin(GL_POINTS);
    for (int i=0; i<LEN(_starsBack); i++) {
        float c = .4 + 0.1*fw_lut_sin(i + time->elapsed*3.f);
        glColor4f(c, c, c, 1);
        glVertex2f(_starsBack[i].x, _starsBack[i].y);
        _starsBack[i].x += 30.f * time->delta;
        if (_starsBack[i].x > FW_RENDER_WIDTH) {
            _starsBack[i].x -= FW_RENDER_WIDTH;
            _starsBack[i].y = FW_RENDER_HEIGHT * ((float)rand() / RAND_MAX);
        }
    }

    for (int i=0; i<LEN(_starsMiddle); i++) {
        float c = .6 + 0.1*fw_lut_sin(1.5f  + i + time->elapsed*5.f);
        glColor4f(c, c, c, 1);
        glVertex2f(_starsMiddle[i].x, _starsMiddle[i].y);
        _starsMiddle[i].x += 40.f * time->delta;
        if (_starsMiddle[i].x > FW_RENDER_WIDTH) {
            _starsMiddle[i].x -= FW_RENDER_WIDTH;
            _starsMiddle[i].y = FW_RENDER_HEIGHT * ((float)rand() / RAND_MAX);
        }
    }

    for (int i=0; i<LEN(_starsFront); i++) {
        float c = .8 + 0.2*fw_lut_sin(i + time->elapsed*8.f);
        glColor4f(c, c, c, 1);
        glVertex2f(_starsFront[i].x, _starsFront[i].y);
        glColor4f(.5, .5, .5, 1);
        glVertex2f(_starsFront[i].x-1, _starsFront[i].y);
        _starsFront[i].x += 80.f * time->delta;
        if (_starsFront[i].x-1 > FW_RENDER_WIDTH) {
            _starsFront[i].x -= FW_RENDER_WIDTH;
            _starsFront[i].y = FW_RENDER_HEIGHT * ((float)rand() / RAND_MAX);
        }
    }

    glEnd();
    glPopMatrix();
}



