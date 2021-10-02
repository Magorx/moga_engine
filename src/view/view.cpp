#include "view.h"


View::View(ViewBody body, RenderableObject *texture, MouseLambda *on_click, MouseLambda *on_hover, MouseLambda *on_release, MouseLambda *on_tick):
body(body),
texture(texture),
on_click(on_click),
on_hover(on_hover),
on_release(on_release),
on_tick(on_tick)
{
    set_layer(1);
    if (texture) {
        texture->set_layer(1);
    }
}

View::~View() {
    for (auto subview : subviews) {
        delete subview;
    }
}

void View::tick(const double, const double) {
    if (texture) {
        texture->set_position(body.get_position());
    }
}

void View::subtick(const double dt, const double time) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->tick(dt, time);
    }
}

void View::render(Renderer *renderer) {
    if (texture) texture->render(renderer);
    subrender(renderer);
}

void View::subrender(Renderer *renderer) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->render(renderer);
    }
}

void View::clicked(Vec2d click) {
    // printf("click %d %d\n", (int) click.x(), (int) click.y());
    if (on_click) (*on_click)(click);

    subclick(click);
}

void View::subclick(Vec2d click) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        Vec2d subpos = subviews[i]->get_body().get_position();

        if (subviews[i]->is_clicked(click - subpos)) {
            subviews[i]->clicked(click - subpos);
        }
    }
}

void View::hovered(Vec2d from, Vec2d to) {
    if (on_hover) (*on_hover)(to);

    subhover(from, to);
}

void View::subhover(Vec2d from, Vec2d to) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        Vec2d subpos = subviews[i]->get_body().get_position();

        if (subviews[i]->is_clicked(to - subpos) || subviews[i]->is_clicked(from - subpos)) {
            subviews[i]->hovered(from - subpos, to - subpos);
        }
    }
}

void View::released(Vec2d release) {
    if (on_release) (*on_release)(release);

    subrelease(release);
}

void View::subrelease(Vec2d release) {
    for (size_t i = 0; i < subviews.size(); ++i) {
        Vec2d subpos = subviews[i]->get_body().get_position();

        if (subviews[i]->is_clicked(release - subpos)) {
            subviews[i]->released(release - subpos);
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
