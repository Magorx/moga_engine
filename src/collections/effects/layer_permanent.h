#pragma once


#include "utils.h"
#include "redactor/layer/layer.h"
#include "redactor/canvas/canvas.h"

#include "redactor/plugin_std.h"
#include "redactor/plugins/settings_window.h"

class PluginEffect : public Effect<Layer> {
protected:
    const char *name;
    PluginSettingsWindow *w_settings;

public:
    PluginEffect(const char *name);

    const char *get_name() const { return name; }

    virtual void apply() override;

    virtual void toggle_settings();
};
