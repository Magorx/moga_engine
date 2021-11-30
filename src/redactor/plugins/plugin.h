#pragma once


#include "redactor/plugin_std.h"
#include "utils/vec2d.h"
#include "visual/color/rgba.h"

#include "redactor/plugins/settings_window.h"


class PluginManager;

class RedactorPlugin {
    void *lib_handle;

    const PPluginInfo      *lib;
    const PPluginInterface *interface;

    PluginSettingsWindow *settings;

    int status;

public:
    RedactorPlugin(const char *fileName, const PAppInterface *appInterface, PluginManager *manager = nullptr);
    ~RedactorPlugin();

    void on_tick(double dt) { interface->general.on_tick(dt); }

    void dump() { interface->general.dump(); }

    PPreviewLayerPolicy get_flush_policy() { return interface->general.get_flush_policy(); }

    void on_mouse_press(const Vec2d &position);
    void on_mouse_move(const Vec2d &from, const Vec2d &to);
    void on_mouse_release(const Vec2d &position);

    void apply();

    bool is_ok() {
        return !status;
    }

    const PPluginInterface *get_inteface() const { return interface; }
    const PPluginInfo *get_lib() const { return lib; }
    PPluginType get_type() const { return lib->type; }

    PluginSettingsWindow *get_settings() { return settings; }
    void set_settings(PluginSettingsWindow *settings_) { settings = settings_; }

    static PVec2f  to_pvec2d(const Vec2d  &vec) { return PVec2f(vec.x(), vec.y()); }
    static Vec2d from_pvec2d(const PVec2f &vec) { return Vec2d (vec.x  , vec.y  ); }

    static PRGBA  to_pcolor(const RGBA  &col) { return col.ui32; }
    static RGBA from_pcolor(const PRGBA &col) { return col.ui32; }
        
};
