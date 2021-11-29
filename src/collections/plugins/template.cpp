#include "redactor/plugin_std.h"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const char *PNAME    = "A name";
const char *PVERSION = "0.1";
const char *PAUTHOR  = "An author";
const char *PDESCR   = "A discription";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_BLEND;

// ============================================================================ Resources

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

static void on_mouse_down(PVec2f pos) {
    draw(pos);
}

static void on_mouse_move(PVec2f /*from*/, PVec2f to) {
    draw(to);
}

static void on_mouse_up(PVec2f /*pos*/) {}

static void apply() {}

static bool enable_extension(const char * /*name*/) {
    return false;
}

static void *get_extension_func(const char * /*name*/) {
    return nullptr;
}

static void draw(PVec2f pos) {
    float size = APPI->general.get_size();

    float a1 = rand();
    float a2 = rand();

    PVec2f p0 = pos;

    PVec2f p1 = {(float) (pos.x + cos(a1) * size), (float) (pos.y + sin(a2) * size)};
    PVec2f p2 = {(float) (pos.x + cos(a2) * size), (float) (pos.y + sin(a1) * size)};

    PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_ACTIVE, nullptr };
    APPI->render.triangle(p0, p1, p2,
                          APPI->general.get_color(),
                          &render_mode);
}
