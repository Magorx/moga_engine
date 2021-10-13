#pragma once


#include "view/view.h"


class v_MouseCatcher;


class MouseCatcherPressAcceptor : public EventAcceptor<v_MouseCatcher, Event::MousePress> {
public:
    MouseCatcherPressAcceptor(v_MouseCatcher *mouse_catcher);

    EventAccResult operator()(const Event::MousePress &event) override;
};

class MouseCatcherMoveAcceptor : public EventAcceptor<v_MouseCatcher, Event::MouseMove> {
public:
    MouseCatcherMoveAcceptor(v_MouseCatcher *mouse_catcher);

    EventAccResult operator()(const Event::MouseMove &event) override;
};


class v_MouseCatcher : public AbstractView {
    Vec2d pos_delta;

    bool captured;

    MouseCatcherPressAcceptor on_press;
    MouseCatcherMoveAcceptor  on_move;

    friend MouseCatcherPressAcceptor;
    friend MouseCatcherMoveAcceptor;

public:
    v_MouseCatcher(const ViewBody &body, AbstractView *parent = nullptr, SmartColor *color=nullptr);

    void press(Vec2d click);
    void move(Vec2d from, Vec2d to);
    void released(Vec2d click);

    void capture();
    void uncapture();
};
