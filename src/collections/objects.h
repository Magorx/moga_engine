#ifndef OBJECTS_COLLECTION_H
#define OBJECTS_COLLECTION_H

#include "engine/object.h"

#include "smart_colors.h"
#include "solid_bodies.h"
#include "renderables.h"

class o_Ball : public Object {
    // r_Circle *texture;
	// sb_Circle *solid_body;

public:
    o_Ball(Vec3d pos, double rad, SmartColor *color, double mass=1);

    virtual void tick(const double = 0, const double = 0) override;
};

#endif // OBJECTS_COLLECTION_H