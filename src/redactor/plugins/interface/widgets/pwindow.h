#pragma once


#include "pabstract.h"


class PluginWindow : public PluginWidget {
    PluginWindow(const ViewBody &body, PluginWidget *parent = nullptr);
    PluginWindow(Widget *widget);

    virtual bool add_child(Widget *child) override;

};
