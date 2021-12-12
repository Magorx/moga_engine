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

struct {
    P::Window *window;
    P::TextField *field;
    P::Slider *slider;
    P::ColorPicker *picker;
    P::Button *button;
} r_settings;

// ============================================================================ General


#include "plugin_interface.h"


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

    if (APPI->factory.widget) {
        r_settings.window = APPI->factory.widget->window("SHRPY", {{100, 100}, {200, 320}});
        r_settings.field = APPI->factory.widget->text_field({{50, 5}, {100, 30}}, r_settings.window);
        r_settings.slider = APPI->factory.widget->slider(P::Slider::Type::X, {{20, 40}, {160, 20}}, r_settings.window);
        r_settings.picker = APPI->factory.widget->color_picker({{0, 70}, {200, 200}}, r_settings.window);
        
        r_settings.button = APPI->factory.widget->button({{75, 275}, {50, 30}}, r_settings.window);

        auto bl = APPI->factory.widget->button({{10, 275}, {50, 30}}, r_settings.window);
        auto br = APPI->factory.widget->button({{140, 275}, {50, 30}}, r_settings.window);

        auto size = fmax(1, r_settings.button->get_body().size.y - 5);
        r_settings.button->set_caption("KCTF", size);
        r_settings.button->set_handler([](){APPI->log("praise the %s", PAUTHOR);});

        r_settings.field->set_text("30");
        r_settings.slider->set_fraction(0.30);

        br->set_caption("<<<", size);
        bl->set_caption(">>>", size);
    }

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status MyPluginInterface::deinit() const {
    if (r_settings.window) {
        r_settings.window->set_to_delete(true);
    }

    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return P::OK;
}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const P::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {
}

void MyPluginInterface::tool_on_press(const P::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const P::Vec2f &/*from*/, const P::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const P::Vec2f &/*pos*/) const {}

void MyPluginInterface::show_settings() const {
    r_settings.window->show();
}

void MyPluginInterface::effect_apply() const {}

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

void MyPluginInterface::draw(const P::Vec2f &pos) const {
    float size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    if (r_settings.window) {
        auto buffer = r_settings.field->get_text().begin();
        auto ms = read_next_long_long(&buffer);
        size = ms * r_settings.slider->get_fraction();
    }

    float a1 = rand();
    float a2 = rand();

    P::Vec2f p0 = pos;
 
    P::Vec2f p1 = {(float) (pos.x + cos(a1) * size), (float) (pos.y + sin(a2) * size)};
    P::Vec2f p2 = {(float) (pos.x + cos(a2) * size), (float) (pos.y + sin(a1) * size)};

    auto target = APPI->get_target();
    
    target->render_triangle(p0, p1, p2, color, P::COPY);
    
    delete target;
}
