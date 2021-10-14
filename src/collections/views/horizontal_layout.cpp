#include "horizontal_layout.h"


v_HorizontalLayout::v_HorizontalLayout(ViewBody body, ViewBody padding_body, 
                                   double padding_split, 
                                   AbstractView *parent,
                                   SmartColor *color,
                                   double highlight_coef) :
AbstractLayoutView(body, padding_body, padding_split, parent, color, highlight_coef)
{}

void v_HorizontalLayout::layout_refit() {
    int v_cnt = weights.size();
    double overall_weight = sum_weight;

    double cur_min_x = fitting_body.position.x();
    double min_y = fitting_body.position.y();
    double size_y = fitting_body.size.y();

    double fit_pixels = fitting_body.size.x() - padding_split * (v_cnt - 1);

    for (int i = 0; i < v_cnt; ++i) {
        double w = weights[i];
        double coef = w / overall_weight;
        int x_pixels = fit_pixels * coef;

        AbstractView *view = subviews[i];
        view->get_body().set_position({cur_min_x, min_y});
        view->get_body().set_size({static_cast<double>(x_pixels), size_y});

        cur_min_x += x_pixels + padding_split;
        fit_pixels -= x_pixels;
        overall_weight -= w;
    }
}

