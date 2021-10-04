#include "toggler.h"


const double TOGGLER_PRESS_SHADOWER = 0.45;
const double TOGGLER_ONLINE_SHADOWER = 0.65;


v_Toggler::v_Toggler(const ViewBody &body, SmartColor *color_border, SmartColor *color_button, 
              Lambda *on, Lambda *off,
              bool online, float button_factor) :
AbstractView(body, nullptr),
pressed(false),
online(online),
color_border(color_border),
color_button(color_button),
on(on),
off(off),
button_factor(button_factor)
{}

void v_Toggler::render(Renderer *renderer) {
    renderer->draw_rectangle(body.position, body.size, color_border->rgb());

    if (!online) {
        color_button->set_rgb(color_button->rgb() * TOGGLER_ONLINE_SHADOWER);
    }
    if (pressed) {
        color_button->set_rgb(color_button->rgb() * TOGGLER_PRESS_SHADOWER);
    }

    renderer->draw_rectangle(body.position + body.size * (1 - button_factor) / 2, body.size * button_factor, color_button->rgb());

    if (!online) {
        color_button->set_rgb(color_button->rgb() / TOGGLER_ONLINE_SHADOWER);
    }
    if (pressed) {
        color_button->set_rgb(color_button->rgb() / TOGGLER_PRESS_SHADOWER);
    }
    
}


void v_Toggler::clicked(Vec2d click) {
    online = online ^ true;
    press();

    if (online) {
        if (on) (*on)();
    } else {
        if (off) (*off)();
    }

    subclick(click);
}

void v_Toggler::hovered(Vec2d from, Vec2d to) {
    if (is_clicked(to) && !is_clicked(from)) {}

    if (is_clicked(from) && !is_clicked(to)) {
        if (pressed) {
            unpress();
        }
    }

    subhover(from, to);
}

void v_Toggler::released(Vec2d click) {
    unpress();

    subrelease(click);
}

void v_Toggler::press() {
    pressed = true;
}

void v_Toggler::unpress() {
    pressed = false;
}
