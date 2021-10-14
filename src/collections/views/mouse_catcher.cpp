#include "mouse_catcher.h"
#include "../renderables/renderables.h"


v_MouseCatcher::v_MouseCatcher(const ViewBody &body, AbstractView *parent, SmartColor *color):
v_Highlighter(body, color, parent, 1),
pos_delta(0, 0),
captured(false),
on_press(this),
on_move(this)
{
    e_mouse_press.add(new MouseCatcherPressAcceptor(this));
    e_mouse_move.add(new MouseCatcherMoveAcceptor(this));

    e_mouse_press.dispatch_order = true;
    e_mouse_release.dispatch_order = true;
    e_mouse_move.dispatch_order = true;
}

v_MouseCatcher::~v_MouseCatcher() {}

void v_MouseCatcher::capture() {
    captured ^= true;
}


MouseCatcherPressAcceptor::MouseCatcherPressAcceptor(v_MouseCatcher *button) : EventAcceptor(button) {}

EventAccResult MouseCatcherPressAcceptor::operator()(const Event::MousePress &event) {
    v_MouseCatcher *mc = acceptor;

    if (mc->captured) {
        EventAccResult res = mc->get_dispatcher<Event::MousePress>().dispatch_to_sub_es(event);
        if (res & EventAccResult::cont) {
            return res;
        }
    }

    if (mc->is_inside(event.position)) {
        mc->capture();
        mc->get_dispatcher<Event::MousePress>().dispatch_to_sub_es(event);
        return (EventAccResult) (EventAccResult::focus | EventAccResult::done);
    }

    mc->captured = false;
    return EventAccResult::stop;
}

MouseCatcherMoveAcceptor::MouseCatcherMoveAcceptor(v_MouseCatcher *button) : EventAcceptor(button) {}

EventAccResult MouseCatcherMoveAcceptor::operator()(const Event::MouseMove &event) {
    if (acceptor->captured) {
        EventAccResult res = acceptor->get_dispatcher<Event::MouseMove>().dispatch_to_sub_es(event);
        return (EventAccResult) (res | EventAccResult::done);
    } else {
        return EventAccResult::stop;
    }
}
