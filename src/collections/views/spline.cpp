#include "spline.h"


v_Spline::v_Spline(const ViewBody &body) :
v_Highlighter(body),
dots({
    // new v_Magnetic({{0 - 2, body.size.y() + 2}, PX_SPLINE_DOT}, {0, body.size}, -1),
    new v_Magnetic({{0 - 1, body.size.y() + 1}, PX_SPLINE_DOT}, {0, body.size}, -1),
    new v_Magnetic({{body.size.x() + 1, 0 - 1}, PX_SPLINE_DOT}, {0, body.size}, -1),
    // new v_Magnetic({{body.size.x() + 2, 0 - 2}, PX_SPLINE_DOT}, {0, body.size}, -1),
    
    new v_Magnetic({{0, body.size.y()}, PX_SPLINE_DOT}, {0, body.size}, PX_SPLINE_MAG_RAD),
    new v_Magnetic({{body.size.x() / 2, body.size.y() / 2}, PX_SPLINE_DOT}, {0, body.size}, PX_SPLINE_MAG_RAD),
    new v_Magnetic({{body.size.x(), 0}, PX_SPLINE_DOT}, {0, body.size}, PX_SPLINE_MAG_RAD),
}),
output(255, 0),

dot_appr(new AppearenceTexture(Resources.texture.dot))
{
    set_appearence(new AppearenceColor({100, 100, 100}));

    dot_appr->set_screen_shift(-PX_SPLINE_DOT / 2);
    for (auto dot : dots) {
        dot->set_appearence(dot_appr);
        add_subview(dot);

        dot->e_fraction_changed.add(new SplineDotChangeAcceptor(this));
    }

    e_mouse_press.add(new SplineSpawnNewDot(this), false);
    e_mouse_release.add(new SplineUncaptureDot(this));

    recalculate_output();
}

v_Spline::~v_Spline() {
    delete dot_appr;
}

void v_Spline::recalculate_output() {
    Interpolator2d inter;
    for (size_t i = 0; i < dots.size(); ++i) {
        auto dot = dots[i];
        Vec2d pos = dot->get_body().position;
        pos.content[1] = body.size.y() - pos.y();
        pos *= 255;
        pos /= body.size;
        inter.add(pos);
    }

    inter.add({inter.data[0].x() - 1, inter.data[0].y() - 1});
    auto data_size = inter.data.size();
    inter.add({inter.data[data_size - 1].x() + 1, inter.data[data_size - 1].y() + 1});

    output[0] = inter.data[0].y();
    output[output.size() - 1] = inter.data[inter.data.size() - 1].y();

    for (int i = 1; i < (int) output.size() - 1; ++i) {
        auto ret = inter[i];
        output[i] = ret.y();
        // printf("%d) %d\n", i, output[i]);
    }
}

void v_Spline::render(Renderer *renderer) {
    v_Highlighter::render(renderer);

    renderer->shift(body.position);

    if (!output.size()) return;

    // for (size_t i = 0; i < output.size() - 1; ++i) {
    //     Vec2d pos1 = {body.size.x() * i / 255, body.size.y() * output[i] / 255};
    //     pos1.content[1] = body.size.y() - pos1.y();

    //     Vec2d pos2 = {body.size.x() * (i + 1) / 255, body.size.y() * output[i + 1] / 255};
    //     pos2.content[1] = body.size.y() - pos2.y();

    //     renderer->draw_line(pos1, pos2, {255, 0, 0});
    // }

    Interpolator2d inter;
    for (size_t i = 0; i < dots.size(); ++i) {
        auto dot = dots[i];
        Vec2d pos = dot->get_body().position;
        // printf("pos %g %g -> ", pos.x(), pos.y());
        pos.content[1] = body.size.y() - pos.y();
        // pos *= 255;
        // pos /= body.size;
        // printf("add %g %g\n", pos.x(), pos.y());
        inter.add(pos);
    }

    inter.add({inter.data[0].x() - 1, inter.data[0].y() - 1});
    auto data_size = inter.data.size();
    inter.add({inter.data[data_size - 1].x() + 1, inter.data[data_size - 1].y() + 1});

    output[0] = inter.data[0].y();
    output[output.size() - 1] = inter.data[inter.data.size() - 1].y();

    for (int i = 1; i < body.size.x(); ++i) {
        auto ret = inter[i];

        ret = Vec2d{ret.x(), body.size.y() - ret.y()};

        renderer->draw_circle(ret, 1, {255, 0, 0});
    }


    renderer->shift(-body.position);
}


SplineSpawnNewDot::SplineSpawnNewDot(v_Spline *spline) : 
EventAcceptor(spline)
{}

EventAccResult SplineSpawnNewDot::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->dot_captured) {
        auto dot = new v_Magnetic({event.position, PX_SPLINE_DOT}, {0, acceptor->get_body().size}, 10);
        dot->set_appearence(acceptor->dot_appr);
        acceptor->dots.push_back(dot);
        acceptor->add_subview(dot);
        dot->e_fraction_changed.add(new SplineDotChangeAcceptor(acceptor));
        acceptor->recalculate_output();

        dot->e_mouse_press.emit(event);
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
