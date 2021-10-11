#pragma once


#include "view/view.h"
#include "text.h"
#include "../renderables/renderables.h"


extern const Vec2d BUTTON_HOVER_POS_DELTA;
extern const Vec2d BUTTON_CLICK_POS_DELTA;
extern double BUTTON_CLICKED_SHADING_COEF;


class v_Button;


class ButtonPressAcceptor : public EventAcceptor<v_Button, Event::MousePress> {
public:
    ButtonPressAcceptor(v_Button *button);

    EventAccResult operator()(const Event::MousePress &event) override;
};

class ButtonReleaseAcceptor : public EventAcceptor<v_Button, Event::MouseRelease> {
public:
    ButtonReleaseAcceptor(v_Button *button);

    EventAccResult operator()(const Event::MouseRelease &event) override;
};

class ButtonMoveAcceptor : public EventAcceptor<v_Button, Event::MouseMove> {
public:
    ButtonMoveAcceptor(v_Button *button);

    EventAccResult operator()(const Event::MouseMove &event) override;
};


class v_Button : public AbstractView {
    Vec2d pos_delta;

    bool pressed;

    ButtonPressAcceptor   on_press;
    ButtonReleaseAcceptor on_release;
    ButtonMoveAcceptor    on_move;


public:
    v_Button(const ViewBody &body, SmartColor *color);

    virtual void tick(const double = 0, const double = 0) override;
    virtual void subrender(Renderer *renderer) override;

    void add_label(const char *text, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr);

    void press(Vec2d click);
    void move(Vec2d from, Vec2d to);
    void released(Vec2d click);

    void press();
    void unpress();

    friend ButtonPressAcceptor;
    friend ButtonReleaseAcceptor;
    friend ButtonMoveAcceptor;
};
