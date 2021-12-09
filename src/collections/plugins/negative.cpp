#include "redactor/plugin_std/std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::EFFECT;

const char *PNAME    = "Negator";
const char *PVERSION = "2.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Negotiates with you colors";

// ============================================================================ Resources

P::Shader *r_shader_neg = nullptr;

// ============================================================================ General

struct MyPluginInterface : public P::PluginInterface {
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

    if (APPI->feature_level & P::SHADER_SUPPORT) {
        r_shader_neg = APPI->factory.shader->compile(
           "uniform sampler2D texture;                                                  \
            void main() {                                                               \
                vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);                     \
                vec4 color = vec4(1.0 - pixel.r, 1.0 - pixel.g, 1.0 - pixel.b, pixel.w);\
                gl_FragColor = color;                                                   \
            }                                                                           \
           ", P::FRAGMENT);
    }

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status MyPluginInterface::deinit() const {
    if (r_shader_neg) {
        APPI->factory.shader->release(r_shader_neg);
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

void MyPluginInterface::tool_on_press(P::Vec2f pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(P::Vec2f /*from*/, P::Vec2f to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(P::Vec2f /*pos*/) const {}

void MyPluginInterface::effect_apply() const {
    auto target = APPI->get_target();

    if (r_shader_neg) {
        APPI->log("awasome shaders!");
        target->apply_shader(r_shader_neg);
        return;
    } else {
        APPI->log("NOT awasome shaders!");
        auto pixels = target->get_pixels();
        size_t w = target->get_size().x;
        size_t h = target->get_size().y;
        size_t sz = w * h;
        for (size_t i = 0; i < sz; ++i) {
            P::RGBA c = pixels[i];
            pixels[i] = {
                (unsigned char) (255 - c.r),
                (unsigned char) (255 - c.g),
                (unsigned char) (255 - c.b),
                c.a,
            };
        }

        P::RenderMode rmode = {P::COPY, nullptr};
        target->render_pixels({0, 0}, pixels, w, h, &rmode);
        APPI->factory.target->release(pixels);
    }

    APPI->factory.target->release(target);
}


void MyPluginInterface::draw(P::Vec2f pos) const {
    float    size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    P::RenderMode rmode = { P::COPY, nullptr };
    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, &rmode);

    APPI->factory.target->release(preview);
}
