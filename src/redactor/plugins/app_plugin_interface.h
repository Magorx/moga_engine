#pragma once

#include "redactor/plugin_std.h"

namespace appintr {

void render_circle(PVec2f position, float radius, PRGBA color, const PRenderMode *render_mode);
void render_line(PVec2f start, PVec2f end, PRGBA color, const PRenderMode *render_mode);
void render_triangle(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode *render_mode);
void render_rectangle(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode *render_mode);
void render_pixels(PVec2f position, const PRGBA *data, size_t width, size_t height, const PRenderMode *render_mode);

void init(PAppInterface *interface);

}
