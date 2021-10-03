#pragma once


#include "view/view.h"
#include "../renderables/renderables.h"


extern const Vec2d BUTTON_HOVER_POS_DELTA;
extern const Vec2d BUTTON_CLICK_POS_DELTA;
extern double BUTTON_CLICKED_SHADING_COEF;


class v_Button : public View {
    char *lable;
    Vec2d pos_delta;

    bool pressed;

    Lambda *click_response;

public:
    v_Button(const ViewBody &body, SmartColor *color, Lambda *click_response = nullptr, char *lable = nullptr);

    virtual void tick(const double = 0, const double = 0) override;

    void bind(Lambda *click_response_);

    virtual void clicked(Vec2d click) override;
    virtual void hovered(Vec2d from, Vec2d to) override;
    virtual void released(Vec2d click) override;

    void press();
    void unpress();
};
