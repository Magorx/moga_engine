#pragma once


#include "utils/vec2d.h"

namespace Event {

struct ControlKeysData {
    bool alt;
    bool ctrl;
    bool shift;
};

struct MousePress {
    Vec2d position;
};

struct MouseRelease {
    Vec2d position;
};

struct MouseMove {
    Vec2d from;
    Vec2d to;
};

class Renderer;
struct RenderCall {
    Renderer *renderer;
};

struct Toggle {
    
};

}
