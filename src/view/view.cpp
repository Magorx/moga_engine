#include "view.h"
#include <memory>
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>


AbstractView::AbstractView(ViewBody body, RenderableObject *texture, AbstractView *parent, bool to_reprioritize_clicks):
body(body),
texture(texture),
parent(parent),
on_press(this),
on_release(this),
on_move(this)
{
    e_mouse_press.add(new AVPressAcceptor(this));
    e_mouse_release.add(new AVReleaseAcceptor(this));
    e_mouse_move.add(new AVMoveAcceptor(this));

    e_mouse_press.set_event_affector([this](const Event::MousePress &event)     { return Event::MousePress   {event.position - this->body.position}; } );
    e_mouse_release.set_event_affector([this](const Event::MouseRelease &event) { return Event::MouseRelease {event.position - this->body.position}; } );
    e_mouse_move.set_event_affector([this](const Event::MouseMove &event)       { return Event::MouseMove    {event.from - this->body.position, event.to - this->body.position}; } );

    if (to_reprioritize_clicks) {
        e_mouse_press.inverse_dispatch_order();
        e_mouse_release.inverse_dispatch_order();
    }

    if (parent) {
        parent->add_subview(this);

        if (body.position.len_squared() < 1.1 && body.size.len_squared() < 1.1) {
            fit(body.position, body.size);
        }
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
    subview->set_parent(this);
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

void AbstractView::fit(const Vec2d &left_up, const Vec2d &right_down, bool absolute_fit) {
    if (absolute_fit) {
        fit_absolute(left_up, right_down);
    } else {
        fit_proportional(left_up, right_down);
    }
}

void AbstractView::fit_proportional(const Vec2d &left_up, const Vec2d &right_down) {
    if (!parent) {
        return;
    }
    
    ViewBody &p_body = parent->get_body();

    body.position = p_body.get_size() * left_up;
    body.size     = p_body.get_size() * (Vec2d(1, 1) - right_down) - body.position;
}

void AbstractView::fit_absolute(const Vec2d &left_up, const Vec2d &right_down) {
    if (!parent) {
        return;
    }

    ViewBody &p_body = parent->get_body();

    body.position = left_up;
    body.size     = p_body.size - right_down - left_up;
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

