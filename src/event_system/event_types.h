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

struct MouseDrag {
    enum Button {
        left,
        right
    };

    Vec2d from;
    Vec2d to;
    Button button;
};

struct RenderCall {
    Renderer *renderer;
};

struct Activator {
    enum State {
        off = 1 << 0,
        on  = 1 << 1,
        toggle = 1 << 2,
        visualy = 1 << 3,
    };


    State mode = State::toggle;
    void *target = nullptr;

    Activator(State mode = State::toggle, void *target = nullptr) : mode(mode), target(target) {}

    inline bool check_target(void *acceptor) const { return !target || target == acceptor; }
};

struct Close {
    int mode = 0;
};

struct ActivityToggle : public Activator {
    using Activator::Activator;
};

struct Clicked {
    
};

struct Changed {
    double fraction;
};

}
