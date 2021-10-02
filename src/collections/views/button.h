#pragma once


#include "view/view.h"
#include "../renderables/renderables.h"


class v_Button : public View {
    char *lable;
    Vec2d pos_delta;

public:
    v_Button(const ViewBody &body, SmartColor *color, MouseLambda *on_click = nullptr, char *lable = nullptr);

    virtual void tick(const double = 0, const double = 0) override;

    void bind(MouseLambda *on_click);

    virtual void clicked(Vec2d click) override;
    virtual void hovered(Vec2d from, Vec2d to) override;
    virtual void released(Vec2d click) override;
};
