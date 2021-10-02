#include "button.h"


v_Button::v_Button(const ViewBody &body, SmartColor *color, MouseLambda *on_click, char *lable):
View(body, new r_Rectangle(body.position, body.size, color), on_click),
lable(lable),
pos_delta(0, 0)
{}

void v_Button::bind(MouseLambda *on_click) {
    set_on_click(on_click);
}

void v_Button::hovered(Vec2d from, Vec2d to) {
    if (is_clicked(to) && !is_clicked(from)) {
        pos_delta += Vec2d(0, -3);
    }

    if (is_clicked(from) && !is_clicked(to)) {
        pos_delta -= Vec2d(0, -3);
    }

    subhover(from, to);
}

void v_Button::clicked(Vec2d click) {
    if (on_click) (*on_click)(click);

    pos_delta += Vec2d(0, 4);
    SmartColor *color = texture->get_color();
    color->set_rgb(color->rgb() * 0.8);

    subclick(click);
}

void v_Button::released(Vec2d click) {
    pos_delta -= Vec2d(0, 4);

    SmartColor *color = texture->get_color();
    color->set_rgb(color->rgb() / 0.8);

    subrelease(click);
}

void v_Button::tick(const double, const double) {
    if (texture) {
        texture->set_position(body.get_position() + pos_delta);
    }
}
