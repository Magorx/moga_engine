#pragma once


#include "pwidget.h"
#include "collections/views/button.h"


constexpr double PBUTTON_PADDING = 4;


class PluginButton : public PluginWidget, public P::Button {
protected:
    P::Button::HandlerType handler;

public:
    PluginButton(const ViewBody &body, P::Widget *parent = nullptr);

    PluginButton(const Vec2d &pos, const char *caption, P::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }
};

class WidgetButton : public v_Button {
    P::Button *widget;

public:
    WidgetButton(const ViewBody &body, P::Button *widget);
    WidgetButton(const Vec2d &pos, const char *caption, P::Button *widget);

    WIDGET_FIX_(P::Button)
};
