#include "vertical_layout.h"


v_VerticalLayout::v_VerticalLayout(ViewBody body, ViewBody padding_body, 
                                   double padding_split, 
                                   AbstractView *parent,
                                   SmartColor *color,
                                   double highlight_coef) :
AbstractLayoutView(body, padding_body, padding_split, parent, color, highlight_coef)
{}

v_VerticalLayout::~v_VerticalLayout() {
    
}

void v_VerticalLayout::layout_refit() {
    int v_cnt = weights.size();
    double overall_weight = sum_weight;

    double cur_min_y = fitting_body.position.y();
    double min_x = fitting_body.position.x();
    double size_x = fitting_body.size.x();

    double fit_pixels = fitting_body.size.y() - padding_split * (v_cnt - 1);

    for (int i = 0; i < v_cnt; ++i) {
        double w = weights[i];
        double coef = w / overall_weight;
        int y_pixels = fit_pixels * coef;

        AbstractView *view = subviews[i];
        view->get_body().set_position({min_x, cur_min_y});
        view->get_body().set_size({size_x, static_cast<double>(y_pixels)});
        view->recalculate_fit_body();
        view->refit();

        cur_min_y += y_pixels + padding_split;
        fit_pixels -= y_pixels;
        overall_weight -= w;
    }
}