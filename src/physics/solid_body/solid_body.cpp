#include "solid_body.h"
#include "sb_circle.h"

SolidBody::SolidBody(SolidBodyType type, Vec3d position, Vec3d velocity, double mass):
type(type),
position(position),
velocity(velocity),
mass(mass)
{}

Collision *SolidBody::collide(SolidBody *other) {
    switch (other->get_type())
    {
    case SolidBodyType::Circle:
        return ((sb_Circle*) this)->collide(other);
    
    default:
        break;
    }
}