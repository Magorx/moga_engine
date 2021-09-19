#include "sb_circle.h"
#include "collision/coll_circle_circle.h"

sb_Circle::sb_Circle(const Vec3d &position, const double radius, const double mass) :
SolidBody(SolidBodyType::Circle, position, {0, 0, 0}, mass)
{}

Collision *sb_Circle::collide(SolidBody *other) {
    switch(other->get_type()) {
        case SolidBodyType::Circle : 
            return CircleCircleCollision::collide(this, (sb_Circle*) other);

        default :
            return nullptr;
    }
}
