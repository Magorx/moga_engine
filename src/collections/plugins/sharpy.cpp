#include "redactor/plugin_std/std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::TOOL;

const char *PNAME    = "Sharpy";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Spawns random triangles in a circle, blends them on active layer";

// ============================================================================ Resources

void *r_max_size_setting = nullptr;
void *r_size_setting = nullptr;
void *r_color_setting = nullptr;

// ============================================================================ General

struct PluginInterface : public P::PluginInterface {
    bool  enable        (const char */*name*/)                            const override { return false;   }
    void *get_func      (const char */*extension*/, const char */*func*/) const override { return nullptr; }
    void *get_interface (const char */*extension*/, const char */*name*/) const override { return nullptr; }

    const P::PluginInfo *get_info() const override;

    P::Status init   (const P::AppInterface*) const override;
    P::Status deinit ()                       const override;
    void      dump   ()                       const override;

    void on_tick(double dt)   const override;

    void effect_apply() const override;

    void tool_on_press  (P::Vec2f position)          const override;
    void tool_on_release(P::Vec2f position)          const override;
    void tool_on_move   (P::Vec2f from, P::Vec2f to) const override;

    void draw(P::Vec2f position) const;
};


const PluginInterface PINTERFACE {};

const P::PluginInfo PINFO =
{
    PSTD_VERSION, // std_version
    nullptr,     // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    nullptr, // icon
    
    PTYPE
};

const P::AppInterface *APPI = nullptr;


extern "C" const P::PluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}
 
// ============================================================================ Logic

P::Status PluginInterface::init(const P::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    if (APPI->feature_level & P::SETTINGS_SUPPORT) {
        // APPI->settings.create_surface(&PINTERFACE, 200, 200);
        // r_max_size_setting = APPI->settings.add(&PINTERFACE, PST_TEXT_LINE, "Max");
        // r_size_setting = APPI->settings.add(&PINTERFACE, PST_SLIDER_1D, "Size");
        // r_color_setting = APPI->settings.add(&PINTERFACE, PST_COLOR_PICKER, "Color");

        APPI->log("[plugin](%s) is happy for your settings support!", PINFO.name);
    } else {
        APPI->log("[plugin](%s) is NOT happy for you not supporting settings!", PINFO.name);
    }

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status PluginInterface::deinit() const {
    if (APPI->feature_level & P::SETTINGS_SUPPORT) {
        // APPI->settings.destroy_surface(&PINTERFACE);
    }

    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return P::OK;
}

void PluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const P::PluginInfo *PluginInterface::get_info() const {
    return &PINFO;
}

void PluginInterface::on_tick(double /*dt*/) const {
}

void PluginInterface::tool_on_press(P::Vec2f pos) const {
    draw(pos);
}

void PluginInterface::tool_on_move(P::Vec2f /*from*/, P::Vec2f to) const {
    draw(to);
}

void PluginInterface::tool_on_release(P::Vec2f /*pos*/) const {}

void PluginInterface::effect_apply() const {}

inline unsigned long long read_next_long_long(const char **buffer) {
    const char *c = *buffer;
    while (c && (*c == ' ' || *c == '\n')) ++c;
 
    unsigned long long l = 0;
    while (*c >= '0' && *c <= '9') {
        l = l * 10 + *c - '0';
        ++c;
    }
 
    *buffer = c;
    return l;
}

unsigned long long read(const char *text) {
    unsigned long long wanted_size = read_next_long_long(&text);
    return wanted_size;
}

void PluginInterface::draw(P::Vec2f pos) const {
    float size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    float a1 = rand();
    float a2 = rand();

    P::Vec2f p0 = pos;
 
    P::Vec2f p1 = {(float) (pos.x + cos(a1) * size), (float) (pos.y + sin(a2) * size)};
    P::Vec2f p2 = {(float) (pos.x + cos(a2) * size), (float) (pos.y + sin(a1) * size)};

    P::RenderMode rmode = { P::COPY, nullptr };

    auto target = APPI->get_target();
    
    target->render_triangle(p0, p1, p2, color, &rmode);
    
    delete target;
}
