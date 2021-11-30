#include "redactor/plugin_std.h"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const PPluginType PTYPE = PPT_EFFECT;

const char *PNAME    = "Bloor";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Blurrrz an image";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_BLEND;

// ============================================================================ Resources

void *r_shader_blur;
void *r_setting_radius;
void *r_setting_power;
void *r_setting_coef;

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
static void *get_extension_func(const char *name);

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

const PPluginInterface *SELF = &PINTERFACE;

const PPluginInfo PINFO =
{
    PSTDVERSION, // std_version
    nullptr,     // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    
    PTYPE
};

const PAppInterface *APPI = nullptr;


extern "C" const PPluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}

static PPluginStatus init(const PAppInterface *app_interface) {
    srand(time(NULL));

    APPI = app_interface;

    if (!(APPI->general.feature_level & PFL_SHADER_SUPPORT) || !(APPI->general.feature_level & PFL_SETTINGS_SUPPORT)) {
        APPI->general.log("[plugin](%s) can't work without shaders and settings support, I'm sorry", PINFO.name);
        return PPS_ERR;
    }

    APPI->settings.create_surface(SELF, 200, 200);
    r_setting_radius = APPI->settings.add(SELF, PST_TEXT_LINE, "Radius");
    r_setting_power = APPI->settings.add(SELF, PST_TEXT_LINE, "Power");
    r_setting_coef = APPI->settings.add(SELF, PST_TEXT_LINE, "Coef");

    r_shader_blur = APPI->shader.compile(
"                                                                             \
 uniform sampler2D texture;                                                   \
                                                                              \
 uniform int ker_size;                                                        \
 uniform float kernel[100];                                                   \
 uniform float tx_w;                                                          \
 uniform float tx_h;                                                          \
                                                                              \
 void main() {                                                                \
     vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);                      \
     vec2 pos = gl_TexCoord[0].xy;                                            \
     vec2 pixel_size = 1.0 / vec2(tx_w, tx_h);                                \
                                                                              \
     vec4 sum_color = vec4(0.0, 0.0, 0.0, 0.0);                               \
     float sum_weight = 0.0;                                                  \
                                                                              \
     int y = -1;                                                              \
     for (int dy = -ker_size; dy <= ker_size; dy = dy + 1) {                  \
         y += 1;                                                              \
         int x = -1;                                                          \
         for (int dx = -ker_size; dx <= ker_size; dx = dx + 1) {              \
             x += 1;                                                          \
             vec2 cp = pos + vec2(dx, dy) * pixel_size;                       \
             vec4 cc = texture2D(texture, cp);                                \
             sum_color = sum_color + cc * kernel[(ker_size * 2 + 1) * y + x]; \
             sum_weight = sum_weight + kernel[(ker_size * 2 + 1) * y + x];    \
         }                                                                    \
     }                                                                        \
                                                                              \
     vec4 color = sum_color / sum_weight;                                     \
     gl_FragColor = color;                                                    \
 }                                                                            \
"
        , PST_FRAGMENT);

    APPI->general.log("[plugin](%s) inited", PINFO.name);
    return PPS_OK;
}

static PPluginStatus deinit() {
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

static void on_mouse_down(PVec2f /*pos*/) {
}

static void on_mouse_move(PVec2f /*from*/, PVec2f /*to*/) {
}

static void on_mouse_up(PVec2f /*pos*/) {}

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

static void apply() {
    size_t w = 0, h = 0;
    APPI->target.get_size(&w, &h);

    PTextFieldSetting field;
    APPI->settings.get(SELF, r_setting_radius, &field);
    int size = read(field.text);

    APPI->settings.get(SELF, r_setting_power, &field);
    float power = read(field.text) / 10;

    APPI->settings.get(SELF, r_setting_coef, &field);
    float coef = read(field.text) / 10;

    int ker_size = size * 2 + 1;

    float *kernel = new float[ker_size * ker_size * 2];
    Vec2f center(size, size);
    int x = -1;
    int y = -1;
    for (int dy = -size; dy <= size; ++dy) {
        y++;
        x = 0;
        for (int dx = -size; dx <= size; ++dx) {
            x++;

            float val = pow(1 - (center - Vec2f(x, y)).len() / ker_size * coef, power);
            val = val > 0 ? val : 0;
            APPI->general.log("x %d y %d val %g", x, y, val);
            kernel[y * ker_size + x] = val;
        }
    }
    
    APPI->shader.set_uniform_int(r_shader_blur, "ker_size", (int) size);
    APPI->shader.set_uniform_float_arr(r_shader_blur, "kernel", kernel, ker_size * ker_size);
    APPI->shader.set_uniform_float(r_shader_blur, "tx_w", (float) w);
    APPI->shader.set_uniform_float(r_shader_blur, "tx_h", (float) h);

    PRenderMode render_mode = { PPBM_COPY, PPDP_ACTIVE, r_shader_blur };
    APPI->shader.apply(&render_mode);

    delete[] kernel;
}

static bool enable_extension(const char * /*name*/) {
    return false;
}

static void *get_extension_func(const char * /*name*/) {
    return nullptr;
}

static void draw(PVec2f pos) {
    float size  = APPI->general.get_size() ;
    PRGBA color = APPI->general.get_color();

    PRenderMode render_mode = { PPBM_COPY, PPDP_PREVIEW, nullptr }; 
    APPI->render.circle(pos, size, color, &render_mode);
}
