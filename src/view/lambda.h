#pragma once


#include "utils/vec2d.h"


class MouseLambda {
public:
    virtual void operator()(const Vec2d &pos) = 0;
};
