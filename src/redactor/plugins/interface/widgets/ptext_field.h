#pragma once


#include "pwidget.h"
#include "collections/views/text_field.h"


class PluginTextField : public PluginWidget, public P::TextField {
protected:
    P::TextField::HandlerType handler;

public:
    PluginTextField(const ViewBody &body, P::Widget *parent = nullptr);

    PluginTextField(const Vec2d &pos, const char *caption, P::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }

    virtual std::string_view get_text() const override;
    virtual void set_text(std::string_view text) override;
};

class WidgetTextField : public v_TextField {
    P::TextField *widget;

public:
    WidgetTextField(const ViewBody &body, P::TextField *widget);
    WidgetTextField(const Vec2d &pos, const char *caption, P::TextField *widget);

    WIDGET_FIX_(P::TextField)
};
