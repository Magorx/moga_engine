#include "magnetic.h"


v_Magnetic::v_Magnetic(const ViewBody &body, const ViewBody &bounds_, double mag_radius, bool to_be_pressed) :
v_Highlighter(body),
bounds({bounds_.position, bounds_.size}),
bounds_offset(bounds_.position - body.position),
to_be_pressed(to_be_pressed),
mag_radius(mag_radius)
{
    e_mouse_press.add(acc_press = new AVMagneticPressAcceptor(this));
    e_mouse_move.add(new AVMagneticMoveAcceptor(this));
    e_mouse_release.add(new AVMagneticReleaseAcceptor(this));
}

void v_Magnetic::update_bounds(const ViewBody &bounds_) {
    bounds = {bounds_offset, bounds_.size};
}

bool v_Magnetic::magnetize_to(const Vec2d &pos, bool to_check_mag_radius) {
    if (to_check_mag_radius && mag_radius == mag_radius && pos.len() > mag_radius) return false;

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
    
    return true;
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


AVMagneticPressAcceptor::AVMagneticPressAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {
    memset(button_mag, 1, sizeof(button_mag));
}

EventAccResult AVMagneticPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!button_mag[(int)event.button]) return EventAccResult::none;

    acceptor->pressed = acceptor->magnetize_to(event.position);
    
    if (acceptor->pressed) {
        return (EventAccResult) (EventAccResult::done | EventAccResult::focus);
    } else {
        return EventAccResult::cont;
    }
}

void AVMagneticPressAcceptor::set_button_mag(Event::MouseButton button, bool flag) {
    button_mag[(int)button] = flag;
}

void AVMagneticPressAcceptor::set_singular_mag_button(Event::MouseButton button) {
    memset(button_mag, 0, sizeof(button_mag));
    set_button_mag(button, true);
}


AVMagneticReleaseAcceptor::AVMagneticReleaseAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticReleaseAcceptor::operator()(const Event::MouseRelease &, const EventAccResult *) {
    acceptor->pressed = false;

    return EventAccResult::cont;
}


AVMagneticMoveAcceptor::AVMagneticMoveAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (!acceptor->to_be_pressed || acceptor->is_pressed()) {
        acceptor->magnetize_to(event.to, false);
    }

    return EventAccResult::cont;
}
