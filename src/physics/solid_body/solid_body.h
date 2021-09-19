#ifndef SOLID_BODY_H
#define SOLID_BODY_H

#include "utils/vec3d.h"
#include "collision/collision.h"

enum class SolidBodyType {
    Circle,
    None
};

class SolidBody {
    SolidBodyType type;
    
    Vec3d position;
    Vec3d velocity;
    double mass;

public:
    SolidBody(SolidBodyType type, Vec3d position = {0, 0, 0}, Vec3d velocity = {0, 0, 0}, double mass = 1);

    bool del_solid;

    Collision *collide(SolidBody *other);
    
    inline SolidBodyType get_type() const { return type; }

    inline void solid_tick(const double dt) { position += velocity * dt; }

    inline void apply_impulse(const Vec3d &impulse) { velocity += impulse / mass; }

    inline Vec3d get_position() { return position; }

    inline void del() { del_solid = true; }
};

#endif // SOLID_BODY_H