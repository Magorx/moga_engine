#include "mouse_catcher.h"
#include "../renderables/renderables.h"


v_MouseCatcher::v_MouseCatcher(const ViewBody &body, AbstractView *parent, SmartColor *color):
AbstractView(body, color ? new r_Rectangle(body.position, body.size, color) : nullptr, parent),
pos_delta(0, 0),
captured(false),
on_press(this),
on_move(this)
{
    e_mouse_press.pop_observer();
    e_mouse_move.pop_observer();

    e_mouse_press.add(&on_press);
    e_mouse_move.add(&on_move);
    e_mouse_release.add(&on_release);
}


void v_MouseCatcher::capture() {
    captured ^= true;
}


MouseCatcherPressAcceptor::MouseCatcherPressAcceptor(v_MouseCatcher *button) : EventAcceptor(button) {}

EventAccResult MouseCatcherPressAcceptor::operator()(const Event::MousePress &event) {
    if (acceptor->is_inside(event.position)) {
        acceptor->capture();
        return (EventAccResult) (EventAccResult::focus | EventAccResult::done);
    } else {
        acceptor->captured = false;
    }

    return EventAccResult::none;
}

MouseCatcherMoveAcceptor::MouseCatcherMoveAcceptor(v_MouseCatcher *button) : EventAcceptor(button) {}

EventAccResult MouseCatcherMoveAcceptor::operator()(const Event::MouseMove &) {
    if (acceptor->captured) {
        return EventAccResult::children_dispatch_only;
    } else {
        return EventAccResult::stop;
    }
}
