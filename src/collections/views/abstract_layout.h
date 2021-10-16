#pragma once


#include "highlighter.h"


class AbstractLayoutView : public v_Highlighter {
protected:
    std::vector<double> weights;
    double sum_weight;

    ViewBody fitting_body_scale;
    ViewBody fitting_body;
    double padding_split;

    virtual void layout_refit() {}
    
    virtual void recalculate_fit_body() override {
        AbstractView::recalculate_fit_body();

        fitting_body.position = body.size * fitting_body_scale.position;
        fitting_body.size     = body.size * (Vec2d(1, 1) - fitting_body_scale.size) - fitting_body.position;        
    }

    virtual void refit() override {
        AbstractView::refit();
        layout_refit();
    }

public:
    AbstractLayoutView(ViewBody body, ViewBody fitting_body_scale = {{0, 0}, {0, 0}}, double padding_split = 0, AbstractView *parent = nullptr, SmartColor *color = nullptr, double highlight_coef = 1):
    v_Highlighter(body, color, parent, highlight_coef),
    sum_weight(0),
    fitting_body_scale(fitting_body_scale),
    fitting_body(fitting_body_scale),
    padding_split(padding_split)
    {
        if (fitting_body_scale.position.len_squared() < 0.001 && fitting_body_scale.size.len_squared() < 0.001) {
            fitting_body = body;
            fitting_body_scale = {{1, 1}, {1, 1}};
        } else {
            fitting_body.position = body.size * fitting_body_scale.position;
            fitting_body.size     = body.size * (Vec2d(1, 1) - fitting_body_scale.size) - fitting_body.position;
        }
    }

    virtual ~AbstractLayoutView() {}

    inline void layout_add(AbstractView *view, double weight = 1) {
        if (weight < 0) return;

        add_subview(view);
        sum_weight += weight;
        weights.push_back(weight);

        layout_refit();
    }

    inline void add_spaceholder(double weight = 1) {
        if (weight < 0) return;

        add_subview(new AbstractView({{0, 0}, {0, 0}}, this));
        sum_weight += weight;
        weights.push_back(weight);
    }
};