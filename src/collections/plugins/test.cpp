#include "redactor/plugin_std/std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::TOOL;

const char *PNAME    = "TEST";
const char *PVERSION = "69";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Performes some testing";

// ============================================================================ Resources

void *r_max_size_setting = nullptr;
void *r_size_setting = nullptr;
void *r_color_setting = nullptr;

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

P::Status MyPluginInterface::deinit() const {
    if (APPI->feature_level & P::SETTINGS_SUPPORT) {
        // APPI->settings.destroy_surface(&PINTERFACE);
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

void MyPluginInterface::tool_on_move(const P::Vec2f &/*from*/, const P::Vec2f &/*to*/) const {
    // draw(to);
}

void MyPluginInterface::tool_on_release(const P::Vec2f &/*pos*/) const {}

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

#include "abstract_widget.h"

class Radio : public AbstractWidget {
    bool on;
    P::RenderTarget *one;
    P::RenderTarget *two;

public:
    Radio(const P::WBody &body, P::Widget *parent = nullptr) :
    AbstractWidget(body, parent),
    on(false)
    {
        texture = one = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, {200, 100, 100});
        two = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, {100, 200, 100});
    }

    virtual void on_mouse_press(const P::Event::MousePress &event) override {
        if (!is_inside(event.position)) {
            return;
        }

        on ^= true;
        if (on) {
            texture = one;
        } else {
            texture = two;
        }
    }
};

void MyPluginInterface::show_settings() const {}


void func() {
    APPI->log("ALOLOLA");
}

void printstr(const char *str) {
    APPI->log(str);
}

void slfn(P::Vec2f frac) {
    printf("frac is %f %f\n", frac.x, frac.y);
}


void MyPluginInterface::draw(const P::Vec2f &pos) const {
    // float size = APPI->get_size();
    // P::RGBA color = APPI->get_color();

    auto layout = APPI->factory.widget->window("opts", {pos, {400, 200}});
    layout->set_name("abobs");

    auto b1 = APPI->factory.widget->button({0, 0}, "TOUCH ME", layout);
    b1->set_handler(func);
    
    P::Vec2f p = b1->get_body().position;
    p.x += b1->get_body().size.x;
    p.y += b1->get_body().size.y;

    auto b2 = APPI->factory.widget->slider(P::Slider::Type::X, {p, {100, 40}}, layout);
    b2->set_base_color({100, 200, 255});
    b2->set_handler(slfn);

    p.x += b2->get_body().size.x;
    p.y += b2->get_body().size.y;

    auto picker = new Radio({p, {100, 100}}, layout);

    layout->show();

    // picker->set_handler([](P::RGBA color){APPI->log("color %d %d %d %d", color.r, color.g, color.b, color.a);});

    // lbl->set_handler([lbl](std::string_view sw){printstr(sw.begin());});
    // lbl->set_text("ABOBA");

    // auto target = APPI->get_target();
    // APPI->factory.target->release(target);
}
