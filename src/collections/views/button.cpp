#include "button.h"


const Vec2d BUTTON_HOVER_POS_DELTA = {0, -3};
const Vec2d BUTTON_CLICK_POS_DELTA = {0, 4};

double BUTTON_CLICKED_SHADING_COEF = 0.8;


v_Button::v_Button(const ViewBody &body, SmartColor *color, AbstractView *parent):
v_Highlighter(body, color, parent),
pos_delta(0, 0),
pressed(false)
{
    e_mouse_press.add(new ButtonPressAcceptor(this));
    e_mouse_move.add(new ButtonMoveAcceptor(this));
    e_mouse_release.add(new ButtonReleaseAcceptor(this));
}

void v_Button::render(Renderer *renderer) {
    body.position += pos_delta;
    v_Highlighter::render(renderer);
    body.position -= pos_delta;
}

void v_Button::add_label(const char *lable, int char_size, SmartColor *font_color, SmartColor *back_color) {
    v_Text *text = new v_Text(ViewBody{body.size / 2, body.size / 2}, lable, char_size, font_color, back_color);

    text->centrized = true;
    add_subview(text);
}

void v_Button::press() {
    pressed = true;

    pos_delta += BUTTON_CLICK_POS_DELTA;

    if (texture) {
        SmartColor *color = texture->get_color();
        color->set_rgb(color->rgb() * BUTTON_CLICKED_SHADING_COEF);
    }
    color->set_rgb(color->rgb() * BUTTON_CLICKED_SHADING_COEF);
}

void v_Button::unpress() {
    pressed = false;

    pos_delta -= BUTTON_CLICK_POS_DELTA;

    if (texture) {
        SmartColor *color = texture->get_color();
        color->set_rgb(color->rgb() / BUTTON_CLICKED_SHADING_COEF);
    }
    color->set_rgb(color->rgb() / BUTTON_CLICKED_SHADING_COEF);
}

void v_Button::tick(const double, const double) {
    if (texture) {
        texture->set_position(body.get_position() + pos_delta);
    }
}


ButtonPressAcceptor::ButtonPressAcceptor(v_Button *button) : EventAcceptor(button) {}

EventAccResult ButtonPressAcceptor::operator()(const Event::MousePress &) {
    if (!acceptor->pressed) {
        acceptor->press();
    }

    return EventAccResult::none;
}


ButtonReleaseAcceptor::ButtonReleaseAcceptor(v_Button *button) : EventAcceptor(button) {}

EventAccResult ButtonReleaseAcceptor::operator()(const Event::MouseRelease &) {
    if (acceptor->pressed) {
        acceptor->unpress();
    }

    return EventAccResult::none;
}


ButtonMoveAcceptor::ButtonMoveAcceptor(v_Button *button) : EventAcceptor(button) {}

EventAccResult ButtonMoveAcceptor::operator()(const Event::MouseMove &event) {
    v_Button *button = acceptor;

    if (button->is_inside(event.to) && !button->is_inside(event.from)) {
        button->pos_delta += BUTTON_HOVER_POS_DELTA;
    }

    if (button->is_inside(event.from) && !button->is_inside(event.to)) {
        if (button->pressed) {
            button->unpress();
        }

        button->pos_delta -= BUTTON_HOVER_POS_DELTA;
    }
    
    return EventAccResult::none;
}
