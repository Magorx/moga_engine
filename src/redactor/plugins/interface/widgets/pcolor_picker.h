#pragma once


#include "pwidget.h"
#include "collections/redactor_views/color_picker.h"


class PluginColorPicker : public PluginWidget, public P::ColorPicker {
protected:
    P::ColorPicker::HandlerType handler;

public:
    PluginColorPicker(const ViewBody &body, P::Widget *parent = nullptr);

    PluginColorPicker(const Vec2d &pos, const char *caption, P::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }

    virtual P::RGBA get_color() override;
    virtual void set_color(P::RGBA color) override;
};

class WidgetColorPicker : public v_ColorPicker {
    P::ColorPicker *widget;

public:
    WidgetColorPicker(const ViewBody &body, P::ColorPicker *widget);

    WIDGET_FIX_(P::ColorPicker)
};
