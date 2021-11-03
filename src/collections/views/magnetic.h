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
    double mag_radius;

public:
    v_Magnetic(const ViewBody &body, const ViewBody &bounds, double mag_radius = NAN, bool to_be_pressed = true);

    bool magnetize_to(const Vec2d &pos, bool to_check_mag_radius = true);

    void update_bounds(const ViewBody &bounds_);

    void shift_with_bounds(const Vec2d &shift);

    Vec2d get_fraction() const;
    void set_fraction(Vec2d fraction);
};
