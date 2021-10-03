#pragma once


#include "utils/vec2d.h"


class Lambda {
public:
    virtual void operator()() = 0;
};

class MouseLambda {
public:
    virtual void operator()(const Vec2d &pos) = 0;
};
