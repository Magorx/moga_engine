#include "redactor/plugin_std/std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::TOOL;

const char *PNAME    = "Rainbow Stamp";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Cute and NOT harmful";

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

// settings
    void show_settings() const override {
        r_settings.window->show();
    }

//additional
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

    if (APPI->factory.widget) {
        r_settings.window = APPI->factory.widget->window("STMP", {{100, 100}, {200, 320}});
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

P::Status PluginInterface::deinit() const {
    if (r_settings.window) {
        r_settings.window->set_to_delete(true);
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

P::RGBA negative(P::RGBA col) {
    return {(unsigned char) (255 - col.r), 
            (unsigned char) (255 - col.g),
            (unsigned char) (255 - col.b),
            col.a};
}

P::RGBA shift(P::RGBA col) {
    return {col.g, col.b, col.r, col.a};
}

void PluginInterface::draw(P::Vec2f pos) const {
    auto preview = APPI->get_preview();

    float size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    if (r_settings.window) {
        auto buffer = r_settings.field->get_text().begin();
        auto ms = read_next_long_long(&buffer);
        size = ms * r_settings.slider->get_fraction();
    }

    int rnd = rand();

    for (int i = 1; i < 50; ++i) {
        P::RGBA col = color;
        if ((i + (rnd % 2)) % 2) {
            col = negative(col);
        }
        for (int j = 0; j < i % 7 + (rnd % 10); ++j) {
            col = shift(col);
        }

        float frac = (float) i / 10;
        frac = exp(frac);

        preview->render_circle(pos, size / frac, col, P::COPY);
    }
    
    delete preview;
}
