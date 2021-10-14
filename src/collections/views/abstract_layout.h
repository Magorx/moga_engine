#pragma once


#include "highlighter.h"


class AbstractLayoutView : public v_Highlighter {
protected:
    std::vector<double> weights;
    double sum_weight;
    
    ViewBody padding_body;
    double padding_split;

    virtual void refit() {}

public:
    AbstractLayoutView(ViewBody body, ViewBody padding_body_ = {{0, 0}, {0, 0}}, double padding_split = 0, AbstractView *parent = nullptr, SmartColor *color = nullptr, double highlight_coef = 1):
    v_Highlighter(body, color, parent, highlight_coef),
    sum_weight(0),
    padding_body(padding_body_),
    padding_split(padding_split)
    {
        if (padding_body.position.len_squared() < 0.001 && padding_body.size.len_squared() < 0.001) {
            padding_body = body;
        } else if (padding_body.position.len_squared() < 1.1 && padding_body.size.len_squared() < 1.1) {
            padding_body.position = body.size * padding_body.position;
            padding_body.size     = body.size * (Vec2d(1, 1) - padding_body.size) - padding_body.position;
        }
    }

    inline void layout_add(AbstractView *view, double weight = 1) {
        if (weight < 0) return;

        add_subview(view);
        sum_weight += weight;
        weights.push_back(weight);

        refit();
    }
};