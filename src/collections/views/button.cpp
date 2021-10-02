#include "button.h"


const Vec2d BUTTON_HOVER_POS_DELTA = {0, -3};
const Vec2d BUTTON_CLICK_POS_DELTA = {0, 4};

double BUTTON_CLICKED_SHADING_COEF = 0.8;


v_Button::v_Button(const ViewBody &body, SmartColor *color, MouseLambda *on_click, char *lable):
View(body, new r_Rectangle(body.position, body.size, color), on_click),
lable(lable),
pos_delta(0, 0),
pressed(false)
{}

void v_Button::bind(MouseLambda *on_click) {
    set_on_click(on_click);
}

void v_Button::press() {
    pressed = true;

    pos_delta += BUTTON_CLICK_POS_DELTA;

    SmartColor *color = texture->get_color();
    color->set_rgb(color->rgb() * BUTTON_CLICKED_SHADING_COEF);
}

void v_Button::unpress() {
    pressed = false;

    pos_delta -= BUTTON_CLICK_POS_DELTA;

    SmartColor *color = texture->get_color();
    color->set_rgb(color->rgb() / BUTTON_CLICKED_SHADING_COEF);
}


void v_Button::hovered(Vec2d from, Vec2d to) {
    if (is_clicked(to) && !is_clicked(from)) {
        pos_delta += BUTTON_HOVER_POS_DELTA;
    }

    if (is_clicked(from) && !is_clicked(to)) {
        if (pressed) {
            unpress();
        }

        pos_delta -= BUTTON_HOVER_POS_DELTA;
    }

    subhover(from, to);
}

void v_Button::clicked(Vec2d click) {
    if (!pressed) {
        press();
        if (on_click) (*on_click)(click);
    }

    subclick(click);
}

void v_Button::released(Vec2d release) {
    if (pressed) {
        if (on_release) (*on_release)(release);
        unpress();
    }

    subrelease(release);
}

void v_Button::tick(const double, const double) {
    if (texture) {
        texture->set_position(body.get_position() + pos_delta);
    }
}
