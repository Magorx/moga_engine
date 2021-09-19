#ifndef RENDERABLES_COLLECTION_H
#define RENDERABLES_COLLECTION_H

#include "visual/renderer/renderable.h"
#include "visual/color/smart_color.h"

#include "utils/vec3d.h"


struct r_Circle : Renderable {
    Vec3d pos;
    double rad;

    SmartColor *color;

    r_Circle(Vec3d pos, double rad, SmartColor *color);

    void render(Renderer *renderer) override;
};


#endif // RENDERABLES_COLLECTION_H