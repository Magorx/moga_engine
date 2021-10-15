#pragma once


#include "utils/vec2d.h"

class Renderer;

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

struct Activator {
    enum State {
        off,
        on,
        toggle,
    };


    State mode = State::toggle;
    void *target = nullptr;

    inline bool check_target(void *acceptor) const { return !target || target == acceptor; }
};

}
