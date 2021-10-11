#pragma once


#include "utils/vec2d.h"
#include "visual/renderer/renderer.h"

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

struct RenderCall {
    Renderer *renderer;
};

}
