#pragma once


#include "highlighter.h"


class v_Magnetic;


class AVMagneticPressAcceptor : public EventAcceptor<v_Magnetic, Event::MousePress> {
    bool button_mag[(int)Event::MouseButton::MAX + 1];
public:
    AVMagneticPressAcceptor(v_Magnetic *magnetic);

    void set_button_mag(Event::MouseButton button, bool flag);

    void set_singular_mag_button(Event::MouseButton button);

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

    AVMagneticPressAcceptor *acc_press;

public:
    v_Magnetic(const ViewBody &body, const ViewBody &bounds, double mag_radius = NAN, bool to_be_pressed = true);

    bool magnetize_to(const Vec2d &pos, bool to_check_mag_radius = true);
    inline bool magnetize_test(const Vec2d &pos) { return !(mag_radius == mag_radius && (pos - body.position).len_squared() > mag_radius * mag_radius); }

    void update_bounds(const ViewBody &bounds_);

    void shift_with_bounds(const Vec2d &shift);

    Vec2d get_fraction() const;
    void set_fraction(Vec2d fraction);

    AVMagneticPressAcceptor *get_acc_press() { return acc_press; }
};
