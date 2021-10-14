#include "hideable.h"


v_Hideable::v_Hideable(const ViewBody &body, SmartColor *color, AbstractView *parent, bool is_shown, double highlight_coef) : 
v_Highlighter(body, color, parent),
is_shown(is_shown)
{
    e_mouse_press.add(new HideablePressAcceptor(this));
    e_mouse_release.add(new HideableReleaseAcceptor(this));
    e_mouse_move.add(new HideableMoveAcceptor(this));

    e_mouse_press.dispatch_order = true;
    e_mouse_release.dispatch_order = true;
    e_mouse_move.dispatch_order = true;
}

void v_Hideable::render(Renderer *renderer) {
    if (!is_active()) {
        return;
    }

    v_Highlighter::render(renderer);
}


HideablePressAcceptor::HideablePressAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideablePressAcceptor::operator()(const Event::MousePress &event) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    return EventAccResult::cont;
}

HideableReleaseAcceptor::HideableReleaseAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableReleaseAcceptor::operator()(const Event::MouseRelease &event) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    return EventAccResult::cont;
}

HideableMoveAcceptor::HideableMoveAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableMoveAcceptor::operator()(const Event::MouseMove &event) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    return EventAccResult::cont;
}
