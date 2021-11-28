#pragma once


#include "redactor/plugin_std.h"
#include "utils/vec2d.h"
#include "visual/color/rgba.h"


class RedactorPlugin {
    void *lib_handle;

    const PPluginInfo      *lib;
    const PPluginInterface *interface;

    int status = 1;

public:
    RedactorPlugin(const char *fileName, const PAppInterface *appInterface);
    ~RedactorPlugin();

    const PPluginInfo *getInfo() { return lib; }

    void on_tick(double dt) { interface->general.on_tick(dt); }

    void dump() { interface->general.dump(); }

    PPreviewLayerPolicy getFlushPolicy() { return interface->general.get_flush_policy(); }

    void on_mouse_press(const Vec2d &position) {
        interface->tool.on_press(to_pvec2d(position));
    }

    void on_mouse_move(const Vec2d &from, const Vec2d &to) {
        interface->tool.on_move(to_pvec2d(from), to_pvec2d(to));
    }

    void on_mouse_release(const Vec2d &position) {
        interface->tool.on_release(to_pvec2d(position));
    }

    void apply() {
        interface->effect.apply();
    }

    bool is_ok() {
        return !status;
    }

    const PPluginInterface *get_inteface() const { return interface; }
    PPluginType get_type() const { return lib->type; }

    static PVec2f  to_pvec2d(const Vec2d  &vec) { return PVec2f(vec.x(), vec.y()); }
    static Vec2d from_pvec2d(const PVec2f &vec) { return Vec2d (vec.x  , vec.y  ); }

    static PRGBA  to_pcolor(const RGBA  &col) { return col.ui32; }
    static RGBA from_pcolor(const PRGBA &col) { return col.ui32; }
        
};
