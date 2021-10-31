#include "button.h"


const Vec2d BUTTON_HOVER_POS_DELTA = {0, -3};
const Vec2d BUTTON_CLICK_POS_DELTA = {0, 4};

double BUTTON_CLICKED_SHADING_COEF = 0.8;


v_Button::v_Button(const ViewBody &body, SmartColor *color, AbstractView *parent, double highlight_coef) :
v_Highlighter(body, color, parent, highlight_coef, true),
pos_delta(0, 0),
style(nullptr)
{
    e_mouse_press.add(new ButtonPressAcceptor(this));
    e_mouse_move.add(new ButtonMoveAcceptor(this));
    e_mouse_release.add(new ButtonReleaseAcceptor(this));
}

v_Button::v_Button(const ViewBody &body, MouseReactionStyle *style, AbstractView *parent) :
v_Highlighter(body, nullptr, parent, 0, true),
pos_delta(0, 0),
style(style)
{
    appearenced = true;

    if (style) {
        auto av_animator_press = new AVAnimatorPress(this, style);
        auto av_animator_release = new AVAnimatorRelease(this, style);
        auto av_animator_move    = new AVAnimatorMove(this, style);
        
        e_mouse_press.add(av_animator_press);
        e_mouse_release.add(av_animator_release);
        e_mouse_move.add(av_animator_move);

        if (style->idle) set_appearence(style->idle);
    }

    e_mouse_press.add(new ButtonPressAcceptor(this));
    e_mouse_release.add(new ButtonReleaseAcceptor(this));
    e_mouse_move.add(new ButtonMoveAcceptor(this));
}

v_Button::~v_Button() {
    // delete style;
}

void v_Button::render(Renderer *renderer) {
    // body.position += pos_delta;

    if (pressed && color) {
        color->set_rgb(color->rgb() * BUTTON_CLICKED_SHADING_COEF);
    }

    v_Highlighter::render(renderer);

    if (pressed && color) {
        color->set_rgb(color->rgb() / BUTTON_CLICKED_SHADING_COEF);
    }

    // body.position -= pos_delta;

    subrender(renderer);
}

void v_Button::press() {
    pressed = true;

    pos_delta += BUTTON_CLICK_POS_DELTA;
}

void v_Button::unpress() {
    pressed = false;

    pos_delta -= BUTTON_CLICK_POS_DELTA;
}

ButtonPressAcceptor::ButtonPressAcceptor(v_Button *button, AVMouseReactionResources *res) : EventAcceptor(button), appr_presed(nullptr) {
    if (res) {
        appr_presed = new AppearenceTexture(res->pressed);
    }
}

EventAccResult ButtonPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    v_Button *button = acceptor;
    if (!button->is_inside(event.position)) return EventAccResult::stop;

    if (!button->pressed) {
        button->press();
    }

    return EventAccResult::cont;
}


ButtonReleaseAcceptor::ButtonReleaseAcceptor(v_Button *button, AVMouseReactionResources *res) : EventAcceptor(button), appr_hovered(nullptr) {
    if (res) {
        appr_hovered = new AppearenceTexture(res->hovered);
    }
}

EventAccResult ButtonReleaseAcceptor::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    v_Button *button = acceptor;
    if (!button->is_inside(event.position)) return EventAccResult::stop;

    if (button->pressed) {
        button->unpress();
        button->e_clicked.emit({});
    }

    return EventAccResult::cont;
}


ButtonMoveAcceptor::ButtonMoveAcceptor(v_Button *button, AVMouseReactionResources *res) : EventAcceptor(button), appr_hovered(nullptr), appr_idle(nullptr) {
    if (res) {
        appr_hovered = new AppearenceTexture(res->hovered);
        appr_idle = new AppearenceTexture(res->idle);
    }
}

EventAccResult ButtonMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
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
