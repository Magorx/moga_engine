#include "renderables.h"


r_Circle::r_Circle(Vec3d pos, double rad, SmartColor *color) :
pos(pos),
rad(rad),
color(color)
{}

void r_Circle::render(Renderer *renderer) {
    renderer->draw_circle(pos, rad, color->rgb());
}
