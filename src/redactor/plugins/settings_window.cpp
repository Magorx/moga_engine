#include "settings_window.h"

#include "engine/moga_engine.h"

#include <cstring>


const double PST_COLOR_PICKER_HEIGHT = 100;


PluginSettingsWindow::PluginSettingsWindow(const char *name, double width, double height) :
v_DialogWindow(name, width),
width(width),
height(height)
{
    toggle_hide_button();
    toggle_close_button();
}

PluginSettingsWindow::~PluginSettingsWindow() {

}


void *PluginSettingsWindow::add_setting(PSettingType type, const char *name) {
    void *ret = nullptr;
    SettingType stype = SettingType::none;

    if (strcmp(type, PST_TEXT_LINE)) {
        ret = add_field(name, width / 2);
        stype = SettingType::text_line;
    } else if (strcmp(type, PST_COLOR_PICKER)) {
        ret = add_color_picker(PST_COLOR_PICKER_HEIGHT);
        stype = SettingType::color_picker;
    } else if (strcmp(type, PST_SLIDER_1D)) {
        ret = add_slider(name);
        stype = SettingType::slider1d;
    }

    if (ret) {
        settings_type_map[ret] = stype;
    }

    return ret;
}

void PluginSettingsWindow::get_setting(void *handle, void *answer) {
    if (!settings_type_map.count(handle)) {
        return;
    }

    AbstractView *av_handle = (AbstractView*) handle; 
    SettingType stype = settings_type_map[handle];

    if (stype == SettingType::text_line) {
        v_TextField *field = dynamic_cast<v_TextField*>(av_handle);
        if (!field) return;

        *((PTextFieldSetting*) answer) = { field->get_string() };
        
        return;
    }

    if (stype == SettingType::slider1d) {
        v_Magnetic *magnetic = dynamic_cast<v_Magnetic*>(av_handle);
        if (!magnetic) return;

        Vec2d frac = magnetic->get_fraction();

        *((PSlider1dSetting*) answer) = { (float) std::max(frac.x(), frac.y()) };
        
        return;
    }

    if (stype == SettingType::color_picker) {
        v_ColorPicker *picker = dynamic_cast<v_ColorPicker*>(av_handle);
        if (!picker) return;

        *((PColorPickerSetting*) answer) = { picker->get_color().ui32 };
        
        return;
    }

}

