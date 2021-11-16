#pragma once


#include "abstract.h"
#include "view_event_types.h"


class ViewEventSystem : public AbstractEventSystem<ViewEventSystem> {
public:
    EventDispatcher<ViewEventSystem, Event::MousePress>      e_mouse_press;
    EventDispatcher<ViewEventSystem, Event::MouseRelease>    e_mouse_release;
    EventDispatcher<ViewEventSystem, Event::MouseMove>       e_mouse_move;
    EventDispatcher<ViewEventSystem, Event::Scroll>          e_scroll;
    EventDispatcher<ViewEventSystem, Event::MouseDrag>       e_mouse_drag;
    EventDispatcher<ViewEventSystem, Event::Activator>       e_toggle;
    EventDispatcher<ViewEventSystem, Event::ActivityToggle>  e_toggle_activity;
    EventDispatcher<ViewEventSystem, Event::RenderCall>      e_render_call;
    EventDispatcher<ViewEventSystem, Event::Close>           e_close;
    EventDispatcher<ViewEventSystem, Event::Clicked>         e_clicked;
    EventDispatcher<ViewEventSystem, Event::FractionChanged> e_fraction_changed;
    EventDispatcher<ViewEventSystem, Event::VectorFractionChanged> e_vec_fraction_changed;

    EventDispatcher<ViewEventSystem, Event::KeyDown>   e_key_down;
    EventDispatcher<ViewEventSystem, Event::KeyUp>     e_key_up;
    EventDispatcher<ViewEventSystem, Event::TextEnter> e_text_enter;

    EventDispatcher<ViewEventSystem, Event::TextChanged> e_text_changed;

    EventDispatcher<ViewEventSystem, Event::DataPtr> e_data_ptr;

    ViewEventSystem() :
    AbstractEventSystem(),
    e_mouse_press(this, "mouse_press"),
    e_mouse_release(this, "mouse_release"),
    e_mouse_move(this, "mouse_move"),
    e_scroll(this, "scroll"),
    e_mouse_drag(this, "mouse_drag"),
    e_toggle(this, "toggle"),
    e_toggle_activity(this, "toggle_activity"),
    e_render_call(this, "render_call"),
    e_close(this, "close"),
    e_clicked(this, "clicked"),
    e_fraction_changed(this, "fraction_changed"),
    e_vec_fraction_changed(this, "vec_fraction_changed"),
    e_key_down(this, "key down"),
    e_key_up(this, "key up"),
    e_text_enter(this, "text enter"),
    e_text_changed(this, "text changed"),
    e_data_ptr(this, "data ptr")
    {}

    template <typename ES_T, typename T>
    EventDispatcher<ES_T, T> &get_dispatcher();
};

template <>
inline EventDispatcher<ViewEventSystem, Event::MousePress> &ViewEventSystem::get_dispatcher() {
    return e_mouse_press;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::MouseRelease> &ViewEventSystem::get_dispatcher() {
    return e_mouse_release;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::MouseMove> &ViewEventSystem::get_dispatcher() {
    return e_mouse_move;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::MouseDrag> &ViewEventSystem::get_dispatcher() {
    return e_mouse_drag;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::Activator> &ViewEventSystem::get_dispatcher() {
    return e_toggle;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::ActivityToggle> &ViewEventSystem::get_dispatcher() {
    return e_toggle_activity;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::RenderCall> &ViewEventSystem::get_dispatcher() {
    return e_render_call;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::Close> &ViewEventSystem::get_dispatcher() {
    return e_close;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::Clicked> &ViewEventSystem::get_dispatcher() {
    return e_clicked;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::FractionChanged> &ViewEventSystem::get_dispatcher() {
    return e_fraction_changed;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::VectorFractionChanged> &ViewEventSystem::get_dispatcher() {
    return e_vec_fraction_changed;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::DataPtr> &ViewEventSystem::get_dispatcher() {
    return e_data_ptr;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::KeyDown> &ViewEventSystem::get_dispatcher() {
    return e_key_down;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::KeyUp> &ViewEventSystem::get_dispatcher() {
    return e_key_up;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::TextEnter> &ViewEventSystem::get_dispatcher() {
    return e_text_enter;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::TextChanged> &ViewEventSystem::get_dispatcher() {
    return e_text_changed;
}

template <>
inline EventDispatcher<ViewEventSystem, Event::Scroll> &ViewEventSystem::get_dispatcher() {
    return e_scroll;
}

#include "dispatcher_end.h"
