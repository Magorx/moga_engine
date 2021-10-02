#include "view.h"


View::View(ViewBody body, RenderableObject *texture, Lambda *reaction):
body(body),
texture(texture),
reaction(reaction)
{}

View::~View() {
    for (auto subview : subviews) {
        delete subview;
    }
}

void View::render(Renderer *renderer) {
    if (texture) texture->render(renderer);
    subrender(renderer);
}

void View::subrender(Renderer *renderer) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->subrender(renderer);
    }
}

void View::on_click(Vec2d click) {
    if (reaction) (*reaction)();

    subclick(click);
}

void View::subclick(Vec2d click) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        if (subviews[i]->clicked(click)) {
            subviews[i]->on_click(click - subviews[i]->get_body().get_position());
        }
    }
}

void View::on_hover(Vec2d from, Vec2d to) {
    if (reaction) (*reaction)();

    subhover(from, to);
}

void View::subhover(Vec2d from, Vec2d to) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        if (subviews[i]->clicked(to) || subviews[i]->clicked(from)) {
            subviews[i]->on_hover(from - subviews[i]->get_body().get_position(), to - subviews[i]->get_body().get_position());
        }
    }
}

void View::on_release(Vec2d release) {
    if (reaction) (*reaction)();

    subrelease(release);
}

void View::subrelease(Vec2d release) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        if (subviews[i]->clicked(release)) {
            subviews[i]->on_release(release - subviews[i]->get_body().get_position());
        }
    }
}

void View::add_subview(View *subview) {
    if (!subview) return;

    subviews.push_back(subview);
}

void View::delete_subview(View *view) {
    size_t i = 0;
    size_t views_cnt = subviews.size();

    for (; i < views_cnt && subviews[i] != view; ++i);

    if (i < views_cnt) {
        delete subviews[i];
        std::swap(subviews[i], subviews[views_cnt - 1]);
        subviews.pop_back();
    }
}

void View::delete_subview(size_t index) {
    if (index < subviews.size()) {
        std::swap(subviews[index], subviews[subviews.size()]);
        subviews.pop_back();
    }
}

ViewBody &View::get_body() {
    return body;
}
