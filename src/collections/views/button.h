#pragma once


#include "highlighter.h"


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


class v_Button : public v_Highlighter {
    Vec2d pos_delta;

    bool pressed;

    friend ButtonPressAcceptor;
    friend ButtonReleaseAcceptor;
    friend ButtonMoveAcceptor;

public:
    v_Button(const ViewBody &body, SmartColor *color, AbstractView *parent = nullptr);

    virtual void tick(const double = 0, const double = 0) override;
    virtual void render(Renderer *renderer) override;
    // virtual void subrender(Renderer *renderer) override;

    void press();
    void unpress();
};
