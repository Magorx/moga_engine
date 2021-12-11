#pragma once


#include "pwidget.h"
#include "collections/views/highlighter.h"


class PluginLabel : public PluginWidget, public P::Label {
public:
    PluginLabel(const char *text, const ViewBody &body, P::Widget *parent = nullptr);

    virtual std::string_view get_text() const override;
    virtual void set_text(std::string_view text) override;
};

class WidgetLabel : public v_Highlighter {
    P::Label *widget;

public:
    WidgetLabel(const char *text, const ViewBody &body, P::Label *widget);

    WIDGET_FIX_(P::Label)
};
