#pragma once


#include "view/view.h"
#include "../renderables/renderables.h"


extern const double TOGGLER_PRESS_SHADOWER;
extern const double TOGGLER_ONLINE_SHADOWER;


class v_Toggler;


class TogglerPressAcceptor : public EventAcceptor<v_Toggler, Event::MousePress> {
public:
    TogglerPressAcceptor(v_Toggler *button);

    EventAccResult operator()(const Event::MousePress &event) override;
};

class TogglerReleaseAcceptor : public EventAcceptor<v_Toggler, Event::MouseRelease> {
public:
    TogglerReleaseAcceptor(v_Toggler *button);

    EventAccResult operator()(const Event::MouseRelease &event) override;
};

class TogglerMoveAcceptor : public EventAcceptor<v_Toggler, Event::MouseMove> {
public:
    TogglerMoveAcceptor(v_Toggler *button);

    EventAccResult operator()(const Event::MouseMove &event) override;
};


class v_Toggler : public AbstractView {
    bool pressed;
    bool online;

    SmartColor *color_border;
    SmartColor *color_button;

    float button_factor;

    TogglerPressAcceptor   on_press;
    TogglerReleaseAcceptor on_release;
    TogglerMoveAcceptor    on_move;

    friend TogglerPressAcceptor;
    friend TogglerReleaseAcceptor;
    friend TogglerMoveAcceptor;

public:
    v_Toggler(const ViewBody &body, SmartColor *color_border, SmartColor *color_button, AbstractView *parent = nullptr, bool online = false, float button_factor = 0.7);

    virtual void render(Renderer *renderer) override;

    void press();
    void unpress();
};
