#pragma once


#include "highlighter.h"


class v_Magnetic;


class AVMagneticPressAcceptor : public EventAcceptor<v_Magnetic, Event::MousePress> {
public:
    AVMagneticPressAcceptor(v_Magnetic *magnetic);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};


class AVMagneticReleaseAcceptor : public EventAcceptor<v_Magnetic, Event::MouseRelease> {
public:
    AVMagneticReleaseAcceptor(v_Magnetic *magnetic);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};


class AVMagneticMoveAcceptor : public EventAcceptor<v_Magnetic, Event::MouseMove> {
public:
    AVMagneticMoveAcceptor(v_Magnetic *magnetic);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class v_Magnetic : public v_Highlighter {
    friend AVMagneticPressAcceptor;
    friend AVMagneticReleaseAcceptor;
    friend AVMagneticMoveAcceptor;

    ViewBody bounds;
    Vec2d bounds_offset;

    bool to_be_pressed;

public:
    v_Magnetic(const ViewBody &body, const ViewBody &bounds, bool to_be_pressed = true);

    void magnetize_to(const Vec2d &pos);

    void update_bounds(const ViewBody &bounds_);
};
