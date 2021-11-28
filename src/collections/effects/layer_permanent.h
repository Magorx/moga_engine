#pragma once


#include "utils.h"
#include "redactor/layer/layer.h"
#include "redactor/canvas/canvas.h"

#include "redactor/plugins/plugin.h"

#include <map>

class v_DialogWindow;

class PluginEffect : public Effect<Layer>, public WindowSetter {
protected:
    enum SettingType {
        text_line,
        slider1d,
        slider2d,
        color_picker,
    };

    const char *name;
    v_DialogWindow *settings_window;

    std::map<void*, SettingType> settings_type_map;

public:
    PluginEffect();

    virtual void apply() override;

    virtual v_Window *create_settings_window(MogaEngine *engine) override;

    void *add_setting(PSettingType type, const char *name);
    void  get_setting(void *handle, void *answer);
};
