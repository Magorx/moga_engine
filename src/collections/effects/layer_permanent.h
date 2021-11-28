#pragma once


#include "utils.h"
#include "redactor/layer/layer.h"
#include "redactor/canvas/canvas.h"

#include "redactor/plugins/plugin.h"
#include "redactor/plugins/settings_window.h"

class PluginEffect : public Effect<Layer>, public WindowSetter {
protected:
    const char *name;
    PluginSettingsWindow *w_settings;

public:
    PluginEffect(const char *name);

    const char *get_name() const { return name; }

    virtual void apply() override;
};
