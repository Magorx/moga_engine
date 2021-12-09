#pragma once


#include "collections/views/dialog_window.h"
class MogaEngine;

#include "redactor/plugin_std/std.hpp"


extern const double PST_COLOR_PICKER_HEIGHT;


class PluginSettingsWindow : public v_DialogWindow {
protected:
    enum SettingType {
        none,
        text_line,
        slider1d,
        slider2d,
        color_picker,
    };

    const char *name;

    double width;
    double height;

    std::map<void*, SettingType> settings_type_map;

public:
    PluginSettingsWindow(const char *name, double width, double height);
    virtual ~PluginSettingsWindow();

    void *add_setting(PSettingType type, const char *name);
    void  get_setting(void *handle, void *answer);
};
