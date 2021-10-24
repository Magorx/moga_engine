#include "view.h"
#include <memory>
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>


AbstractView::AbstractView(ViewBody body, AbstractView *parent):
view_id(nullptr),
body(body),
fit_body(body),
parent(parent),
appearence(nullptr),
pressed(false),
focuseable(false),
appearenced(false)
{
    e_render_call.add(new AVRenderCallAcceptor(this));
    
    e_mouse_press.add(new AVPressFocuser(this));

    e_mouse_press.add(new AVCoveredPressBlocker(this));
    e_mouse_move.add(new AVCoveredMoveBlocker(this));

    e_mouse_press.set_event_affector([this](const Event::MousePress &event)     { return Event::MousePress   {event.position - this->body.position}; } );
    e_mouse_release.set_event_affector([this](const Event::MouseRelease &event) { return Event::MouseRelease {event.position - this->body.position}; } );
    e_mouse_move.set_event_affector([this](const Event::MouseMove &event)       { return Event::MouseMove    {event.from - this->body.position, event.to - this->body.position}; } );


    if (parent) {
        parent->add_subview(this);

        if (body.position.len_squared() < 2) {
            if (body.size.len_squared() < 2) {
                fit(fit_body.position, fit_body.size);
            } else {
                body.position = parent->get_body().get_size() * body.position;
            }
        } else {
            Vec2d p_size = parent->get_body().size;
            fit_body.position = body.position / p_size;
            fit_body.size = (body.position + body.size) / p_size;
        }
    }
}

AbstractView::~AbstractView() {
}

void AbstractView::render(Renderer *renderer) {
    subrender(renderer);
}

void AbstractView::subrender(Renderer *renderer) {
    renderer->shift(body.position);

    e_render_call.dispatch_to_sub_es({renderer}, true);

    renderer->shift(-body.position);
}

void AbstractView::add_subview(AbstractView *subview) {
    if (!subview) return;

    subviews.push_back(subview);
    add_es(subview);
    subview->set_parent(this);
}

void AbstractView::delete_subview(AbstractView *view) {
    size_t i = 0;
    size_t views_cnt = subviews.size();

    for (; i < views_cnt && subviews[i] != view; ++i);

    if (i < views_cnt) {
        printf("CHECK delete_subview IF FAILS\n");
        // delete subviews[i];
        std::swap(subviews[i], subviews[views_cnt - 1]);
        subviews.pop_back();
    }
}

void AbstractView::delete_subview(size_t index) {
    if (index < subviews.size()) {
        std::swap(subviews[index], subviews[subviews.size()]);
        subviews.pop_back();
    }
}

ViewBody &AbstractView::get_body() {
    return body;
}

void AbstractView::fit(const Vec2d &left_up, const Vec2d &right_down, bool absolute_fit) {
    if (absolute_fit) {
        fit_absolute(left_up, right_down);
    } else {
        fit_proportional(left_up, right_down);
    }
    
    recalculate_fit_body();
    refit();
}

void AbstractView::fit_proportional(const Vec2d &left_up, const Vec2d &right_down) {
    if (!parent) {
        return;
    }
    
    ViewBody &p_body = parent->get_body();

    body.position = p_body.get_size() * left_up;
    body.size     = p_body.get_size() * (right_down - left_up);
}

void AbstractView::fit_absolute(const Vec2d &left_up, const Vec2d &right_down) {
    if (!parent) {
        return;
    }

    body.position = left_up;
    body.size     = right_down - body.position;

    recalculate_fit_body();
}

void AbstractView::refit() {
    refit_in_parent();
    for (auto subview : subviews) {
        subview->refit();
    }
}

void AbstractView::refit_in_parent() {
    fit_proportional(fit_body.position, fit_body.size);
}

void AbstractView::recalculate_fit_body() {
    if (!parent) return;

    Vec2d p_size = parent->get_body().size;
    fit_body.position = body.position / p_size;
    fit_body.size = (body.position + body.size) / p_size;
}


AVMissPressBlocker::AVMissPressBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissPressBlocker::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.position)) {
        return EventAccResult::stop;
    } else {
        acceptor->pressed = true;
    }

    return EventAccResult::cont;
}


AVMissReleaseBlocker::AVMissReleaseBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissReleaseBlocker::operator()(const Event::MouseRelease &, const EventAccResult *) {
    acceptor->pressed = false;
 
    // if (!acceptor->is_inside(event.position)) {
    //     return EventAccResult::stop;
    // }

    return EventAccResult::none;
}


AVMissMoveBlocker::AVMissMoveBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissMoveBlocker::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.from) && !acceptor->is_inside(event.to)) {
        return EventAccResult::stop;
    }
    
    return EventAccResult::cont;
}

AVRenderCallAcceptor::AVRenderCallAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVRenderCallAcceptor::operator()(const Event::RenderCall &event, const EventAccResult *) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    acceptor->render(event.renderer);

    return EventAccResult::stop;
}

AVDragAcceptor::AVDragAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVDragAcceptor::operator()(const Event::MouseDrag &event, const EventAccResult *) {
    acceptor->get_body().position += event.to - event.from;

    return EventAccResult::stop;
}

AVDragEmitter::AVDragEmitter(AbstractView *av) : EventAcceptor(av) {
    av->e_mouse_press.add(new AVMissPressBlocker(av));
    av->e_mouse_release.add(new AVMissReleaseBlocker(av));
}

EventAccResult AVDragEmitter::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (acceptor->pressed) {
        acceptor->e_mouse_drag.emit({event.from, event.to, Event::MouseDrag::left});
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

AVPressFocuser::AVPressFocuser(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVPressFocuser::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (acceptor->is_inside(event.position) && acceptor->is_focuseable()) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::focus | EventAccResult::prevent_siblings_dispatch);
    } else {
        return EventAccResult::none;
    }
}

AVCoveredMoveBlocker::AVCoveredMoveBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredMoveBlocker::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (acceptor->is_inside(event.from) && acceptor->is_inside(event.to)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }
    
    return EventAccResult::none;
}

AVCloseGenerator::AVCloseGenerator(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCloseGenerator::operator()(const Event::MousePress &, const EventAccResult *) {
    acceptor->e_close.emit({});
    return EventAccResult::cont;
}


AVCloseAcceptor::AVCloseAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCloseAcceptor::operator()(const Event::Close &, const EventAccResult *) {  
    acceptor->to_delete = true;
    return EventAccResult::none;
}

AVCoveredPressBlocker::AVCoveredPressBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredPressBlocker::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (acceptor->is_inside(event.position)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

AVCoveredReleaseBlocker::AVCoveredReleaseBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredReleaseBlocker::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    if (acceptor->is_inside(event.position)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

