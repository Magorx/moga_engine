#include "renderables.h"


r_Circle::r_Circle(Vec3d pos, double rad, SmartColor *color) :
pos(pos),
rad(rad),
color(color)
{}

void r_Circle::render(Renderer *renderer) {
    renderer->draw_circle(pos, rad, color->rgb());
}

r_Line::r_Line(Vec3d p1, Vec3d p2, SmartColor *color) :
p1(p1),
p2(p2),
color(color)
{}

void r_Line::render(Renderer *renderer) {
    renderer->draw_line(p1, p2, color->rgb());
}
