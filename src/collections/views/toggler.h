#pragma once


#include "view/view.h"
#include "../renderables/renderables.h"


extern const double TOGGLER_PRESS_SHADOWER;
extern const double TOGGLER_ONLINE_SHADOWER;


class v_Toggler : public View {
    bool pressed;
    bool online;

    SmartColor *color_border;
    SmartColor *color_button;

    Lambda *on;
    Lambda *off;

    float button_factor;

public:
    v_Toggler(const ViewBody &body, SmartColor *color_border, SmartColor *color_button, 
              Lambda *on = nullptr, Lambda *off = nullptr,
              bool online = false, float button_factor = 0.7);

    // virtual void tick(const double = 0, const double = 0) override;

    virtual void render   (Renderer *renderer) override;

    void bind(Lambda *on_click);

    virtual void clicked(Vec2d click) override;
    virtual void hovered(Vec2d from, Vec2d to) override;
    virtual void released(Vec2d click) override;

    void press();
    void unpress();
};
