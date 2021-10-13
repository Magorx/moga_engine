#include "view.h"
#include <memory>
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>


AbstractView::AbstractView(ViewBody body, RenderableObject *texture, bool to_reprioritize_clicks):
body(body),
texture(texture),
on_press(this),
on_release(this),
on_move(this)
{
    e_mouse_press.add(&on_press);
    e_mouse_release.add(&on_release);
    e_mouse_move.add(&on_move);

    e_mouse_press.set_event_affector([this](const Event::MousePress &event)     { return Event::MousePress   {event.position - this->body.position}; } );
    e_mouse_release.set_event_affector([this](const Event::MouseRelease &event) { return Event::MouseRelease {event.position - this->body.position}; } );
    e_mouse_move.set_event_affector([this](const Event::MouseMove &event)       { return Event::MouseMove    {event.from - this->body.position, event.to - this->body.position}; } );

    if (to_reprioritize_clicks) {
        e_mouse_press.inverse_dispatch_order();
        e_mouse_release.inverse_dispatch_order();
    }
}

AbstractView::~AbstractView() {
    for (auto subview : subviews) {
        delete subview;
    }
}


void AbstractView::tick(const double, const double) {
    if (texture) {
        texture->set_position(body.get_position());
    }
}

void AbstractView::subtick(const double dt, const double time) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->tick(dt, time);
    }
}

void AbstractView::render(Renderer *renderer) {
    if (texture) texture->render(renderer);
    subrender(renderer);
}

void AbstractView::subrender(Renderer *renderer) {
    renderer->shift(body.position);

    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->render(renderer);
    }

    renderer->shift(-body.position);
}

void AbstractView::add_subview(AbstractView *subview) {
    if (!subview) return;

    subviews.push_back(subview);
    add(subview);
}

void AbstractView::delete_subview(AbstractView *view) {
    size_t i = 0;
    size_t views_cnt = subviews.size();

    for (; i < views_cnt && subviews[i] != view; ++i);

    if (i < views_cnt) {
        delete subviews[i];
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

AVPressAcceptor::AVPressAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVPressAcceptor::operator()(const Event::MousePress &event) {
    if (!acceptor->is_inside(event.position)) {
        return EventAccResult::stop;
    }

    return EventAccResult::none;
}


AVReleaseAcceptor::AVReleaseAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVReleaseAcceptor::operator()(const Event::MouseRelease &event) {
    if (!acceptor->is_inside(event.position)) {
        return EventAccResult::stop;
    }

    return EventAccResult::none;
}


AVMoveAcceptor::AVMoveAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMoveAcceptor::operator()(const Event::MouseMove &event) {
    if (!acceptor->is_inside(event.from) && !acceptor->is_inside(event.to)) {
        return EventAccResult::stop;
    }
    
    return EventAccResult::none;
}

