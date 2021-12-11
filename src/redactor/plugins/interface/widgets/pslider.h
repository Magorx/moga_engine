#pragma once


#include "pwidget.h"
#include "collections/views/magnetic.h"


constexpr double PSLIDER_DOT_PADDING = 4;


class PluginSlider : public PluginWidget, public P::Slider {
protected:
    P::Slider::HandlerType handler;
    P::Slider::Type type;

public:
    PluginSlider(P::Slider::Type type, const ViewBody &body, P::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }

    virtual P::Vec2f get_fraction_2d() override;
    virtual void set_fraction_2d(P::Vec2f frac) override;

    virtual float get_fraction() override;
    virtual void set_fraction(float frac) override;
};

class WidgetSlider : public v_Magnetic {
    P::Slider *widget;

public:
    WidgetSlider(const ViewBody &body, P::Slider *widget);

    WIDGET_FIX_(P::Slider)
};
