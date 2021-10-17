#include "hideable.h"


v_Hideable::v_Hideable(const ViewBody &body, AbstractView *parent, bool to_pass_inactive, bool is_shown) : 
AbstractView(body, parent),
_is_active(is_shown),
to_pass_inactive(to_pass_inactive)
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

EventAccResult HideablePressAcceptor::operator()(const Event::MousePress &, const EventAccResult*) {
    if (!acceptor->is_active() && !acceptor->to_pass_inactive) return EventAccResult::stop;

    return EventAccResult::none;
}

HideableReleaseAcceptor::HideableReleaseAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableReleaseAcceptor::operator()(const Event::MouseRelease &, const EventAccResult*) {
    if (!acceptor->is_active() && !acceptor->to_pass_inactive) return EventAccResult::stop;

    return EventAccResult::none;
}

HideableMoveAcceptor::HideableMoveAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableMoveAcceptor::operator()(const Event::MouseMove &, const EventAccResult*) {
    if (!acceptor->is_active() && !acceptor->to_pass_inactive) return EventAccResult::stop;

    return EventAccResult::none;
}

HideableActivityToggleAcceptor::HideableActivityToggleAcceptor(v_Hideable *hideable) : EventAcceptor(hideable) {}

EventAccResult HideableActivityToggleAcceptor::operator()(const Event::ActivityToggle &event, const EventAccResult *) {
    v_Hideable *hid = acceptor;
    printf("got active: %s\n", event.mode == Event::ActivityToggle::State::off ? "off" : "NOT off");
    if (!event.check_target(hid)) return EventAccResult::none;
    printf("yes\n");

    if (event.mode == Event::Activator::State::on) {
        hid->activate();
    } else if (event.mode == Event::Activator::State::off) {
        printf("WELL\n");
        hid->deactivate();
    } else if (event.mode == Event::Activator::State::toggle) {
        hid->toggle();
    }

    return EventAccResult::done;
}

