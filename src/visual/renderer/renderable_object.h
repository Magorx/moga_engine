#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H


#include "renderable.h"
#include "utils/vec3d.h"


class RenderableObject : public Renderable {
protected:
    Vec3d position;

public:
    RenderableObject() : position(Vec3d{0, 0, 0}) {}
    RenderableObject(Vec3d position) : position(position) {}

    inline Vec3d get_position() const { return position; }
    inline void set_position(const Vec3d &position_) { position = position_; }
};

#endif // RENDERABLE_OBJECT_H
