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

    if (!(APPI->general.feature_level & PFL_SHADER_SUPPORT)) {
        APPI->general.log("[plugin](%s) can't work without shaders and settings support, I'm sorry", PINFO.name);
        return PPS_ERR;
    }
    
    if (APPI->general.feature_level & PFL_SETTINGS_SUPPORT) {

        APPI->settings.create_surface(SELF, 200, 200);
        r_setting_radius = APPI->settings.add(SELF, PST_TEXT_LINE, "Radius");
        r_setting_power = APPI->settings.add(SELF, PST_TEXT_LINE, "Power");
        r_setting_coef = APPI->settings.add(SELF, PST_TEXT_LINE, "Coef");
    }

    r_shader_blur = APPI->shader.compile(
"                                                                                  \
 uniform sampler2D texture;                                                        \
                                                                                   \
 uniform int ker_size;                                                             \
 uniform float kernel[10000];                                                      \
 uniform float tx_w;                                                               \
 uniform float tx_h;                                                               \
                                                                                   \
 vec3 rgb2hsl(vec3 color) {                                                        \
  	vec3 hsl;                                                                       \
                                                                                   \
  	float fmin = min(min(color.r, color.g), color.b);                               \
  	float fmax = max(max(color.r, color.g), color.b);                               \
  	float delta = fmax - fmin;                                                      \
                                                                                   \
  	hsl.z = (fmax + fmin) / 2.0;                                                    \
                                                                                   \
  	if (delta == 0.0)                                                               \
  	{                                                                               \
  		hsl.x = 0.0;                                                                   \
  		hsl.y = 0.0;                                                                   \
  	} else                                                                          \
  	{                                                                               \
  		if (hsl.z < 0.5)                                                               \
  			hsl.y = delta / (fmax + fmin);                                                \
  		else                                                                           \
  			hsl.y = delta / (2.0 - fmax - fmin);                                          \
                                                                                   \
  		float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;             \
  		float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;             \
  		float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;             \
                                                                                   \
  		if (color.r == fmax)                                                           \
  			hsl.x = deltaB - deltaG;                                                      \
  		else if (color.g == fmax)                                                      \
  			hsl.x = (1.0 / 3.0) + deltaR - deltaB;                                        \
  		else if (color.b == fmax)                                                      \
  			hsl.x = (2.0 / 3.0) + deltaG - deltaR;                                        \
                                                                                   \
  		if (hsl.x < 0.0)                                                               \
  			hsl.x += 1.0;                                                                 \
  		else if (hsl.x > 1.0)                                                          \
  			hsl.x -= 1.0;                                                                 \
  	}                                                                               \
                                                                                   \
  	return hsl;                                                                     \
  }                                                                                \
                                                                                   \
                                                                                   \
                                                                                   \
 float hue2rgb(float f1, float f2, float hue) {                                    \
     if (hue < 0.0)                                                                \
         hue += 1.0;                                                               \
     else if (hue > 1.0)                                                           \
         hue -= 1.0;                                                               \
     float res;                                                                    \
     if ((6.0 * hue) < 1.0)                                                        \
         res = f1 + (f2 - f1) * 6.0 * hue;                                         \
     else if ((2.0 * hue) < 1.0)                                                   \
         res = f2;                                                                 \
     else if ((3.0 * hue) < 2.0)                                                   \
         res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;                         \
     else                                                                          \
         res = f1;                                                                 \
     return res;                                                                   \
 }                                                                                 \
                                                                                   \
 vec3 hsl2rgb(vec3 hsl) {                                                          \
     vec3 rgb;                                                                     \
                                                                                   \
     if (hsl.y == 0.0) {                                                           \
         rgb = vec3(hsl.z);                                                        \
     } else {                                                                      \
         float f2;                                                                 \
                                                                                   \
         if (hsl.z < 0.5)                                                          \
             f2 = hsl.z * (1.0 + hsl.y);                                           \
         else                                                                      \
             f2 = hsl.z + hsl.y - hsl.y * hsl.z;                                   \
                                                                                   \
         float f1 = 2.0 * hsl.z - f2;                                              \
                                                                                   \
         rgb.r = hue2rgb(f1, f2, hsl.x + (1.0/3.0));                               \
         rgb.g = hue2rgb(f1, f2, hsl.x);                                           \
         rgb.b = hue2rgb(f1, f2, hsl.x - (1.0/3.0));                               \
     }                                                                             \
     return rgb;                                                                   \
 }                                                                                 \
                                                                                   \
 vec3 rgb2hsv(vec3 c)                                                              \
 {                                                                                 \
     vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);                              \
     vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));             \
     vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));             \
                                                                                   \
     float d = q.x - min(q.w, q.y);                                                \
     float e = 1.0e-10;                                                            \
     return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);      \
 }                                                                                 \
                                                                                   \
 vec3 hsv2rgb(vec3 c)                                                              \
 {                                                                                 \
     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);                                \
     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);                             \
     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);                     \
 }                                                                                 \
                                                                                   \
 void main() {                                                                     \
     vec2 pos = gl_TexCoord[0].xy;                                                 \
     vec2 pixel_size = 1.0 / vec2(tx_w, tx_h);                                     \
                                                                                   \
     vec3 sum_color = vec3(0.0, 0.0, 0.0);                                         \
     float sum_weight = 0.0;                                                       \
                                                                                   \
     for (int dy = -ker_size, y = 0; dy <= ker_size; dy += 1, y += 1) {            \
         for (int dx = -ker_size, x = 0; dx <= ker_size; dx = dx + 1, x += 1) {    \
             vec2  cur_pos = pos + vec2(dx, dy) * pixel_size;                      \
             vec4  cur_col = texture2D(texture, cur_pos);                          \
             float cur_coef = kernel[(ker_size * 2 + 1) * y + x];                  \
                                                                                   \
             sum_color = sum_color + cur_col.xyz * cur_coef;              \
             sum_weight = sum_weight + cur_coef;                                   \
         }                                                                         \
     }                                                                             \
                                                                                   \
     vec3 blur_color = sum_color / sum_weight;                                     \
     vec3 init_color = texture2D(texture, gl_TexCoord[0].xy).xyz;         \
                                                                                   \
     vec3 color = init_color - (blur_color - init_color) * 0.1;                    \
                                                                                   \
     vec3 final = vec3(init_color.xy, color.z);                                    \
                                                                                   \
     gl_FragColor = vec4(blur_color, texture2D(texture, gl_TexCoord[0].xy).w); \
 }                                                                                 \
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
#include <cstdio>
float read_float(const char *text) {
    float val = 0;
    int symbs_read = 0;
    sscanf(text, "%g%n", &val, &symbs_read);
    return val;
}

static void apply() {
    size_t w = 0, h = 0;
    APPI->target.get_size(&w, &h);
    
    int size = 3;
    float power = 1.3;
    float coef = 1.2;

    PTextFieldSetting field;
    if (r_setting_radius) {
        APPI->settings.get(SELF, r_setting_radius, &field);
        size = read(field.text);
    }

    if (r_setting_power) {
        APPI->settings.get(SELF, r_setting_power, &field);
        power = read_float(field.text);
        if (power < 0.01) {
            power = 1;
        }
    }

    if (r_setting_coef) {
        APPI->settings.get(SELF, r_setting_coef, &field);
        coef = read_float(field.text);
        if (coef < 0.01) {
            coef = 1;
        }
    }

    int ker_size = size * 2 + 1;

    float *kernel = new float[ker_size * ker_size * 2];
    Vec2f center(size, size);
    
    int y = -1;
    for (int dy = -size; dy <= size; ++dy) {
        y++;
        int x = -1;
        for (int dx = -size; dx <= size; ++dx) {
            x++;
            float val = pow(1 - (center - Vec2f(x, y)).len() / (ker_size * coef), power);
            val = val > 0 ? val : 0;
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
