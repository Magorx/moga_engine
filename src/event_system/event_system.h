#pragma once


#include "event.h"
#include "event_types.h"


class EventSystem {
public:
    EventDispatcher<Event::MousePress>   e_mouse_press;
    EventDispatcher<Event::MouseRelease> e_mouse_release;
    EventDispatcher<Event::MouseMove>    e_mouse_move;

    EventSystem() :
    e_mouse_press("mouse_press"),
    e_mouse_release("mouse_release"),
    e_mouse_move("mouse_hover")
    {}

    void add_es(EventSystem *sub_system) {
        if (!sub_system) return;

        e_mouse_press   .add(&sub_system->e_mouse_press);
        e_mouse_release .add(&sub_system->e_mouse_release);
        e_mouse_move    .add(&sub_system->e_mouse_move);
    }
};
