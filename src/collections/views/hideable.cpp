#include "hideable.h"


v_Hideable::v_Hideable(const ViewBody &body, AbstractView *parent, bool to_pass_inactive, bool is_shown) : 
AbstractView(body, parent),
to_pass_inactive(to_pass_inactive),
_is_active(is_shown)
{
    e_mouse_press.add(new HideablePressAcceptor(this));
    e_mouse_release.add(new HideableReleaseAcceptor(this));
    e_mouse_move.add(new HideableMoveAcceptor(this));
}

void v_Hideable::render(Renderer *renderer) {
    if (!is_active()) {
        return;
    }

    AbstractView::render(renderer);
}


HideablePressAcceptor::HideablePressAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideablePressAcceptor::operator()(const Event::MousePress &) {
    if (!acceptor->is_active() && !acceptor->to_pass_inactive) return EventAccResult::stop;

    return EventAccResult::none;
}

HideableReleaseAcceptor::HideableReleaseAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableReleaseAcceptor::operator()(const Event::MouseRelease &) {
    if (!acceptor->is_active() && !acceptor->to_pass_inactive) return EventAccResult::stop;

    return EventAccResult::none;
}

HideableMoveAcceptor::HideableMoveAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableMoveAcceptor::operator()(const Event::MouseMove &) {
    if (!acceptor->is_active() && !acceptor->to_pass_inactive) return EventAccResult::stop;

    return EventAccResult::none;
}

HideableActivatorAcceptor::HideableActivatorAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableActivatorAcceptor::operator()(const Event::Activator &event) {
    v_Hideable *hid = acceptor;
    if (!event.check_target(hid)) return EventAccResult::none;

    if (event.on == Event::Activator::State::on) {
        hid->deactivate();
    } else if (event.on == Event::Activator::State::off) {
        hid->activate();
    } else {
        hid->toggle();
    }

    return EventAccResult::done;
}

