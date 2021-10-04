#include "button.h"


const Vec2d BUTTON_HOVER_POS_DELTA = {0, -3};
const Vec2d BUTTON_CLICK_POS_DELTA = {0, 4};

double BUTTON_CLICKED_SHADING_COEF = 0.8;


v_Button::v_Button(const ViewBody &body, SmartColor *color, Lambda *click_response):
AbstractView(body, new r_Rectangle(body.position, body.size, color)),
pos_delta(0, 0),
pressed(false),
click_response(click_response)
{}

void v_Button::subrender(Renderer *renderer) {
    renderer->shift(body.position + pos_delta);

    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->render(renderer);
    }

    renderer->shift(-body.position - pos_delta);
}

void v_Button::bind(Lambda *click_response_) {
    click_response = click_response_;
}

void v_Button::add_label(const char *lable, int char_size, SmartColor *font_color, SmartColor *back_color) {
    v_Text *text = new v_Text(ViewBody{body.size / 2, body.size / 2}, lable, char_size, font_color, back_color);

    text->centrized = true;
    add_subview(text);
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
        if (click_response) (*click_response)();
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
