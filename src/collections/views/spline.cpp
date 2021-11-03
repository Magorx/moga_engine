#include "spline.h"


v_Spline::v_Spline(const ViewBody &body) :
v_Highlighter(body),
dots({
    new v_Magnetic({{0, body.size.y()}, PX_SPLINE_DOT}, body, -1),
    new v_Magnetic({{0, body.size.y()}, PX_SPLINE_DOT}, body, PX_SPLINE_MAG_RAD),
    new v_Magnetic({{body.size.x(), 0}, PX_SPLINE_DOT}, body, PX_SPLINE_MAG_RAD),
    new v_Magnetic({{body.size.x(), 0}, PX_SPLINE_DOT}, body, -1),
}),
output(255, 0),

dot_appearence(new AppearenceTexture(Resources.texture.dot))
{
    for (auto dot : dots) {
        dot->set_appearence(dot_appearence);
        add_subview(dot);
    }
}

v_Spline::~v_Spline() {
    delete dot_appearence;
}

void v_Spline::recalculate_output() {

}

void v_Spline::render(Renderer *renderer) {
    subrender(renderer);
    AbstractLabledView::render(renderer);
}
