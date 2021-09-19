#include "objects.h"

o_Ball::o_Ball(Vec3d pos, double rad, SmartColor *color, double mass):
Object(new r_Circle(pos, rad, color),
       new sb_Circle(pos, rad, mass))
{}

void o_Ball::tick(const double, const double) {
    texture->pos = solid_body->get_position();
}
