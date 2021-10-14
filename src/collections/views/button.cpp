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
    // body.position += pos_delta;
    v_Highlighter::render(renderer);
    // body.position -= pos_delta;
}

void v_Button::press() {
    pressed = true;

    pos_delta += BUTTON_CLICK_POS_DELTA;

    color->set_rgb(color->rgb() * BUTTON_CLICKED_SHADING_COEF);
}

void v_Button::unpress() {
    pressed = false;

    pos_delta -= BUTTON_CLICK_POS_DELTA;

    color->set_rgb(color->rgb() / BUTTON_CLICKED_SHADING_COEF);
}

ButtonPressAcceptor::ButtonPressAcceptor(v_Button *button) : EventAcceptor(button) {}

EventAccResult ButtonPressAcceptor::operator()(const Event::MousePress &event) {
    if (!acceptor->is_inside(event.position)) return EventAccResult::stop;

    if (!acceptor->pressed) {
        acceptor->press();
    }

    return EventAccResult::cont;
}


ButtonReleaseAcceptor::ButtonReleaseAcceptor(v_Button *button) : EventAcceptor(button) {}

EventAccResult ButtonReleaseAcceptor::operator()(const Event::MouseRelease &event) {
    if (!acceptor->is_inside(event.position)) return EventAccResult::stop;

    if (acceptor->pressed) {
        acceptor->unpress();
    }

    return EventAccResult::cont;
}


ButtonMoveAcceptor::ButtonMoveAcceptor(v_Button *button) : EventAcceptor(button) {}

EventAccResult ButtonMoveAcceptor::operator()(const Event::MouseMove &event) {
    v_Button *button = acceptor;
    if (!button->is_inside(event.to, event.from)) return EventAccResult::none;

    if (button->is_inside(event.to) && !button->is_inside(event.from)) {
        button->pos_delta += BUTTON_HOVER_POS_DELTA;
    }

    if (button->is_inside(event.from) && !button->is_inside(event.to)) {
        if (button->pressed) {
            button->unpress();
        }

        button->pos_delta -= BUTTON_HOVER_POS_DELTA;
    }
    
    return EventAccResult::cont;
}
