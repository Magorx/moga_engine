#include "spline.h"


v_Spline::v_Spline(const ViewBody &body, RGBA curve_color, double mag_radius) :
v_Highlighter(body),
dots({
    // new v_Magnetic({{0 - 2, body.size.y() + 2}, PX_SPLINE_DOT}, {0, body.size}, -1),
    new v_Magnetic({{0, body.size.y()}, PX_SPLINE_DOT}, {{0, 0}, {0, body.size.y()}}, PX_SPLINE_MAG_RAD),
    new v_Magnetic({{body.size.x(), 0}, PX_SPLINE_DOT}, {{body.size.x(), 0}, {0, body.size.y()}}, PX_SPLINE_MAG_RAD),
    // new v_Magnetic({{body.size.x() + 2, 0 - 2}, PX_SPLINE_DOT}, {0, body.size}, -1),
    
    // new v_Magnetic({{0, body.size.y()}, PX_SPLINE_DOT}, {0, body.size}, PX_SPLINE_MAG_RAD),
    new v_Magnetic({{body.size.x() / 2, body.size.y() / 2}, PX_SPLINE_DOT}, {0, body.size - 1}, PX_SPLINE_MAG_RAD),
    // new v_Magnetic({{body.size.x(), 0}, PX_SPLINE_DOT}, {0, body.size}, PX_SPLINE_MAG_RAD),
}),
output(this->body.size.x(), 0),

dot_appr(new AppearenceTexture(Resources.texture.dot)),

curve_color(curve_color),
mag_radius(mag_radius)
{
    e_mouse_press.add(new AVMissPressBlocker(this));

    set_appearence(new AppearenceColor({100, 100, 100, 100}));

    dot_appr->set_screen_shift(-PX_SPLINE_DOT / 2);
    
    for (auto dot : dots) {
        dot->set_appearence(dot_appr);
        add_subview(dot);

        dot->e_fraction_changed.add(new SplineDotChangeAcceptor(this));

        dot->get_acc_press()->set_singular_mag_button(Event::MouseButton::left);
    }

    e_mouse_press.add(new SplineSpawnNewDot(this), false);
    e_mouse_release.add(new SplineUncaptureDot(this));

    recalculate_output();
}

v_Spline::~v_Spline() {
    delete dot_appr;
}

void v_Spline::recalculate_output() {
    Interpolator2dCatmullRom inter;
    for (size_t i = 0; i < dots.size(); ++i) {
        auto dot = dots[i];
        Vec2d pos = dot->get_body().position;
        pos.content[1] = body.size.y() - pos.y();
        inter.add_p(pos);
    }

    inter.add_p({inter.data[0].x() - 1, inter.data[0].y() - 1});
    auto data_size = inter.data.size();
    inter.add_p({inter.data[data_size - 1].x() + 1, inter.data[data_size - 1].y() + 1});

    std::vector<Vec2d> points;

    for (int i = 0; i < body.size.x(); ++i) {
        auto ret = inter[i];

        ret = Vec2d{ret.x(), body.size.y() - ret.y()};
        ret.clamp({0, 0}, body.size - 1);
        points.push_back(ret);
    }

    for (int i = 0; i < (int) points.size() - 1; ++i) {
        const Vec2d p1 = points[i];
        const Vec2d p2 = points[i + 1];
        Vec2d dir = (p2 - p1).normal();
        for (Vec2d p = p1; (p1 - p).len_squared() < (p1 - p2).len_squared(); p += dir / 5) {
            output[p.x()] = body.size.y() - p.y();
        }
    }

    Vec2d p0 = body.size - dots[0]->get_body().position;
    Vec2d p1 = body.size - dots[1]->get_body().position;
    p0.clamp({0, 0}, body.size - 1);
    p1.clamp({0, 0}, body.size - 1);

    output[0] = p0.y();
    output[output.size() - 1] = p1.y();

    e_vec_fraction_changed.emit({output, body.size.x()});
}

void v_Spline::render(Renderer *renderer) {
    v_Highlighter::render(renderer);

    renderer->shift(body.position);

    if (!output.size()) return;

    for (size_t i = 0; i < output.size() - 1; ++i) {
        Vec2d pos1 = {body.size.x() * i / output.size(), body.size.y() * output[i] / output.size()};
        pos1.content[1] = body.size.y() - pos1.y();

        Vec2d pos2 = {body.size.x() * (i + 1) / output.size(), body.size.y() * output[i + 1] / output.size()};
        pos2.content[1] = body.size.y() - pos2.y();

        renderer->draw_line(pos1, pos2, curve_color);
    }

    renderer->shift(-body.position);
}

v_Magnetic *v_Spline::try_spawn_dot(const Vec2d &pos) {
    if (mag_radius == mag_radius) { // check if pos is somewhere near our curve
        bool flag = false;
        for (size_t i = 0; i < output.size(); ++i) {
            if (mag_radius * mag_radius > (pos - Vec2d{(double) i, body.size.y() - output[i]}).len_squared()) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return nullptr;
        }
    }


    auto dot = new v_Magnetic({pos, PX_SPLINE_DOT}, {0, get_body().size - 1}, 10);

    dot->set_appearence(dot_appr);
    dot->e_fraction_changed.add(new SplineDotChangeAcceptor(this));
    dot->get_acc_press()->set_singular_mag_button(Event::MouseButton::left);

    dots.push_back(dot);
    add_subview(dot);

    return dot;
}

bool v_Spline::try_delete_dot(const Vec2d &pos) {
    v_Magnetic *dot = nullptr;
    for (size_t i = 2; i < dots.size(); ++i) {
        if (dots[i]->magnetize_test(pos)) {
            dot = dots[i];
            dots.erase(dots.begin() + i);
            recalculate_output();

            dot->to_delete = true;  
            return true;
        }
    }
    
    return false;
}


SplineSpawnNewDot::SplineSpawnNewDot(v_Spline *spline) : 
EventAcceptor(spline)
{}

EventAccResult SplineSpawnNewDot::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (event.button == Event::MouseButton::left) {
        if (!acceptor->dot_captured) {
            auto dot = acceptor->try_spawn_dot(event.position);
            if (dot) {
                acceptor->recalculate_output();
                dot->e_mouse_press.emit(event);
                return EventAccResult::done;
            }
        }
    } else if (event.button == Event::MouseButton::right) {
        if (acceptor->try_delete_dot(event.position)) {
            return EventAccResult::done;
        }
    }

    acceptor->dot_captured = false;

    return EventAccResult::cont;
}


SplineUncaptureDot::SplineUncaptureDot(v_Spline *spline) : 
EventAcceptor(spline)
{}

EventAccResult SplineUncaptureDot::operator()(const Event::MouseRelease &, const EventAccResult *) {
    acceptor->dot_captured = false;

    return EventAccResult::cont;
}


SplineDotChangeAcceptor::SplineDotChangeAcceptor(v_Spline *spline) : 
EventAcceptor(spline)
{}

EventAccResult SplineDotChangeAcceptor::operator()(const Event::FractionChanged &, const EventAccResult *) {
    acceptor->dot_captured = true;
    acceptor->recalculate_output();

    return EventAccResult::cont;
}
