#pragma once


#include "abstract_layout.h"


class v_VerticalLayout : public AbstractLayoutView {
    virtual void refit() override;

public:
    v_VerticalLayout(ViewBody body, ViewBody padding_body = {{0, 0}, {0, 0}}, double padding_split = 0, AbstractView *parent = nullptr, SmartColor *color = nullptr, double highlight_coef = 1);
};
