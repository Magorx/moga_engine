#include "redactor/plugin_std/plugin_std.hpp"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const char *PNAME    = "Neg.Brush";
const char *PVERSION = "version";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "NOT cute and harmful";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_BLEND;

// ============================================================================ Resources

void *r_shader_brush_neg = nullptr;

// ============================================================================


static PPluginStatus init(const PAppInterface* appInterface);
static PPluginStatus deinit();

static void dump();
static void on_tick(double dt);
static void on_update();

static const PPluginInfo  *get_info();
static PPreviewLayerPolicy get_flush_policy();

static void on_mouse_down(PVec2f pos);
static void on_mouse_move(PVec2f from, PVec2f to);
static void on_mouse_up  (PVec2f pos);
static void apply();

static bool  enable_extension  (const char *name);
static void *get_extension_func(const char */*ext*/, const char */*name*/);

static void draw(PVec2f pos);


const PPluginInterface PINTERFACE =
{
    0, // std_version
    0, // reserved
    
    {
        enable_extension,
        get_extension_func,
    },

    // general
    {
        get_info,
        init,
        deinit,
        dump,
        on_tick,
        on_update,
        get_flush_policy,
    },

    // effect
    {
        apply,
    },

    // tool
    {
        on_mouse_down,
        on_mouse_up  ,
        on_mouse_move,
    },
};

const PPluginInfo PINFO =
{
    PSTDVERSION, // std_version
    nullptr,     // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    
    PPT_TOOL
};

const PAppInterface *APPI = nullptr;


extern "C" const PPluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}

static PPluginStatus init(const PAppInterface *app_interface) {
    srand(time(NULL));

    APPI = app_interface;

    if (APPI->general.feature_level & PFL_SHADER_SUPPORT) {

        r_shader_brush_neg = APPI->shader.compile(
           "uniform sampler2D texture;                                  \
            uniform float r;                                            \
            uniform float g;                                            \
            uniform float b;                                            \
            void main() {                                               \
                vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);     \
                vec4 color = vec4(1.0 - r, 1.0 - g, 1.0 - b, pixel.w);  \
                gl_FragColor = color;                                   \
            }                                                           \
           ", PST_FRAGMENT);
    }

    if (!r_shader_brush_neg) {
        APPI->general.log("It is sad that you don't support shaders...");
    }

    APPI->general.log("[plugin](%s) inited", PINFO.name);
    return PPS_OK;
}

static PPluginStatus deinit() {
    if (r_shader_brush_neg) {
        APPI->shader.release(r_shader_brush_neg);
    }
    APPI->general.log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PPS_OK;
}

static void dump() {
    APPI->general.log("[plugin](%s) is active", PINFO.name);
}

static const PPluginInfo *get_info() {
    return &PINFO;
}

static void on_tick(double /*dt*/) {
}

static void on_update() {
}

static PPreviewLayerPolicy get_flush_policy() {
    return FLUSH_POLICY;
}

static void on_mouse_down(PVec2f pos) {
    draw(pos);
}

static void on_mouse_move(PVec2f /*from*/, PVec2f to) {
    draw(to);
}

static void on_mouse_up(PVec2f /*pos*/) {}

static void apply() {
}

static bool enable_extension(const char * /*name*/) {
    return false;
}

static void *get_extension_func(const char */*ext*/, const char */*name*/) {
    return nullptr;
}

PRGBA negative(PRGBA col) {
    return {(unsigned char) (255 - col.r), 
            (unsigned char) (255 - col.g),
            (unsigned char) (255 - col.b),
            col.a};
}

static void draw(PVec2f pos) {
    float size = APPI->general.get_size();
    PRGBA color = APPI->general.get_color();

    if (!r_shader_brush_neg) {
        PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_PREVIEW, nullptr };
        APPI->render.circle(pos, size, negative(color), &render_mode);
    } else {
        PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_PREVIEW, r_shader_brush_neg };
        APPI->shader.set_uniform_float(r_shader_brush_neg, "r", (float) color.r / 255);
        APPI->shader.set_uniform_float(r_shader_brush_neg, "g", (float) color.g / 255);
        APPI->shader.set_uniform_float(r_shader_brush_neg, "b", (float) color.b / 255);
        APPI->render.circle(pos, size, color, &render_mode);
    }
}
