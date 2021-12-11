#pragma once


#include "pwidget.h"
#include "collections/views/window.h"


class PluginWindow : public PluginWidget, public P::Window {
protected:
    P::Window::HandlerType handler_show;
    P::Window::HandlerType handler_hide;

public:
    PluginWindow(const char *name, const ViewBody &body, P::Widget *parent = nullptr);

    virtual void set_show_handler(HandlerType &handler_) override { handler_show = handler_; };
    virtual HandlerType &get_show_handler() override { return handler_show; };

    virtual void set_hide_handler(HandlerType &handler_) override { handler_hide = handler_; };
    virtual HandlerType &get_hide_handler() override { return handler_hide; };

    virtual bool set_name(const char *name) override;
    virtual const char *get_name() override;
};

class WidgetWindow : public v_Window {
    P::Window *widget;

public:
    WidgetWindow(const char *name, const ViewBody &body, P::Window *widget);

    WIDGET_FIX_(P::Window)
};
