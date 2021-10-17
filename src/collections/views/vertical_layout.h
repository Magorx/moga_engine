#pragma once


#include "abstract_layout.h"


class v_VerticalLayout : public AbstractLayoutView {
protected:
    virtual void layout_refit() override;

    void recalculate_size();

public:
    v_VerticalLayout(ViewBody body, ViewBody padding_body = {{0, 0}, {0, 0}}, double padding_split = 0, AbstractView *parent = nullptr, SmartColor *color = nullptr, double highlight_coef = 1);
    virtual ~v_VerticalLayout();
};
