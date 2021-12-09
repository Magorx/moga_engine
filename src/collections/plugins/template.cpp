#include "redactor/plugin_std/std.hpp"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::TOOL; // P::EFFECT

const char *PNAME    = "a_name";
const char *PVERSION = "a_vesion";
const char *PAUTHOR  = "an_author";
const char *PDESCR   = "a_description";

// ============================================================================ Resources

// ============================================================================ General

struct MyPluginInterface : public P::PluginInterface {
    bool  enable        (const char */*name*/)                            const override { return false;   }
    void *get_func      (const char */*extension*/, const char */*func*/) const override { return nullptr; }
    void *get_interface (const char */*extension*/, const char */*name*/) const override { return nullptr; }

    const P::PluginInfo *get_info() const override;

    P::Status init   (const P::AppInterface*) const override;
    P::Status deinit ()                       const override;

    void dump() const override;

    void on_tick(double dt) const override;

// effect
    void effect_apply() const override;

// tool
    void tool_on_press  (P::Vec2f position)          const override;
    void tool_on_release(P::Vec2f position)          const override;
    void tool_on_move   (P::Vec2f from, P::Vec2f to) const override;

// additional
    void draw(P::Vec2f position) const;
};


const MyPluginInterface PINTERFACE {};

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

P::Status MyPluginInterface::init(const P::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status MyPluginInterface::deinit() const {
    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return P::OK;
}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const P::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {}

void MyPluginInterface::tool_on_press(P::Vec2f pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(P::Vec2f /*from*/, P::Vec2f to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(P::Vec2f /*pos*/) const {}

void MyPluginInterface::effect_apply() const {}


void MyPluginInterface::draw(P::Vec2f pos) const {
    float    size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    P::RenderMode rmode = { P::COPY, nullptr };
    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, &rmode);

    APPI->factory.target->release(preview);
}
