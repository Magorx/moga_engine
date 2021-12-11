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

void PluginInterface::tool_on_move(P::Vec2f /*from*/, P::Vec2f /*to*/) const {
    // draw(to);
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

class MyWidget : public P::Widget {
public:
    MyWidget(const P::WBody &body, P::Widget *parent = nullptr) : Widget(body, parent) {}
    virtual ~MyWidget() {}

    virtual bool is_active() override { return true; };
    virtual bool is_inside(P::Vec2f /*pos*/) override { return true; }

    virtual bool add_child(Widget */*child*/) override { return false; }
    virtual bool delete_child(Widget */*child*/) override { return false; }

    virtual bool delete_from_parent() override { return false; }
    
    virtual void on_render          (const P::Event::Render          &) override {}
    virtual void on_tick            (const P::Event::Tick            &) override {}

    virtual void on_mouse_press     (const P::Event::MousePress      &event) override {
        APPI->log("HELLO %lg %lg\n", event.position.x, event.position.y);
    }

    virtual void on_mouse_release   (const P::Event::MouseRelease    &) override {}
    virtual void on_mouse_move      (const P::Event::MouseMove       &) override {}
    virtual void on_key_down        (const P::Event::KeyDown         &) override {}
    virtual void on_key_up          (const P::Event::KeyUp           &) override {}
    virtual void on_text_enter      (const P::Event::TextEnter       &) override {}
    virtual void on_scroll          (const P::Event::Scroll          &) override {}
    virtual void on_hide            (const P::Event::Hide            &) override {}
    virtual void on_show            (const P::Event::Show            &) override {}

    virtual void hide() override {}
    virtual void show() override {}

    virtual void set_caption(const char */*text*/, size_t /*font_size*/, const P::Vec2f */*pos*/ = nullptr) override {}
    virtual void set_base_color(P::RGBA /*color*/) override {};
};


void func() {
    APPI->log("ALOLOLA");
}

void printstr(const char *str) {
    APPI->log(str);
}

void slfn(P::Vec2f frac) {
    printf("frac is %f %f\n", frac.x, frac.y);
}


void PluginInterface::draw(P::Vec2f pos) const {
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

    auto picker = APPI->factory.widget->color_picker({p, {100, 100}}, layout);

    picker->set_handler([](P::RGBA color){APPI->log("color %d %d %d %d", color.r, color.g, color.b, color.a);});

    // lbl->set_handler([lbl](std::string_view sw){printstr(sw.begin());});
    // lbl->set_text("ABOBA");

    // auto target = APPI->get_target();
    // APPI->factory.target->release(target);
}
