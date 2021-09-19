#ifndef CIRCLE_SB_H
#define CIRCLE_SB_H

#include "solid_body.h"

class sb_Circle : public SolidBody {
    double radius;

public:
    sb_Circle(const Vec3d &position, const double radius, const double mass=1);

    Collision *collide(SolidBody *other);

    inline double get_radius() const { return radius; }
};

#endif // CIRCLE_SB_H