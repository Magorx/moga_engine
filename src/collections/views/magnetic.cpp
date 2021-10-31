#include "magnetic.h"


v_Magnetic::v_Magnetic(const ViewBody &body, const ViewBody &bounds, bool to_be_pressed) :
v_Highlighter(body),
bounds({0, bounds.size}),
bounds_offset(bounds.position - body.position),
to_be_pressed(to_be_pressed)
{
    e_mouse_press.add(new AVMagneticPressAcceptor(this));
    e_mouse_move.add(new AVMagneticMoveAcceptor(this));
    e_mouse_release.add(new AVMagneticReleaseAcceptor(this));
}

void v_Magnetic::update_bounds(const ViewBody &bounds_) {
    bounds = {bounds_offset, bounds_.size};
}

void v_Magnetic::magnetize_to(const Vec2d &pos) {
    Vec2d shift = pos;
    Vec2d bound_pos = shift - bounds_offset;

    Vec2d allowed_pos = {
        fmin(fmax(bound_pos.content[0], bounds.position.content[0]), bounds.position.content[0] + bounds.size.content[0]),
        fmin(fmax(bound_pos.content[1], bounds.position.content[1]), bounds.position.content[1] + bounds.size.content[1])
    };

    body.position = allowed_pos;
    bounds_offset = bounds.position - body.position;

    e_fraction_changed.emit({{bounds.size.x() ? body.position.x() / bounds.size.x() : 0,
                              bounds.size.y() ? body.position.y() / bounds.size.y() : 0}});
}

void v_Magnetic::shift_with_bounds(const Vec2d &shift) {
    body.position   += shift;
    bounds.position += shift;
}

Vec2d v_Magnetic::get_fraction() const { 
    return {bounds.size.x() ? body.position.x() / bounds.size.x() : 0,
            bounds.size.y() ? body.position.y() / bounds.size.y() : 0};
}

void v_Magnetic::set_fraction(Vec2d fraction) {
    fraction.content[0] = fmin(fmax(fraction.content[0], 0), 1);
    fraction.content[1] = fmin(fmax(fraction.content[1], 0), 1);

    body.position = bounds.position + bounds.size * fraction;

    bounds_offset = bounds.position - body.position;
}


AVMagneticPressAcceptor::AVMagneticPressAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    acceptor->magnetize_to(event.position);
    acceptor->pressed = true;

    return EventAccResult::cont;
}


AVMagneticReleaseAcceptor::AVMagneticReleaseAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticReleaseAcceptor::operator()(const Event::MouseRelease &, const EventAccResult *) {
    acceptor->pressed = false;

    return EventAccResult::cont;
}


AVMagneticMoveAcceptor::AVMagneticMoveAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (!acceptor->to_be_pressed || acceptor->is_pressed()) {
        acceptor->magnetize_to(event.to);
    }

    return EventAccResult::cont;
}
