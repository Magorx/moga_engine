#include "redactor/plugin_std/std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::TOOL;

const char *PNAME    = "a_name";
const char *PVERSION = "a_vesion";
const char *PAUTHOR  = "an_author";
const char *PDESCR   = "a_description";

// ============================================================================ Resources

void *r_max_size_setting = nullptr;
void *r_size_setting     = nullptr;
void *r_color_setting    = nullptr;

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

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status PluginInterface::deinit() const {
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


void PluginInterface::draw(P::Vec2f pos) const {
    float    size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    P::RenderMode rmode = { P::COPY, nullptr };
    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, &rmode);

    APPI->factory.target->release(preview);
}
