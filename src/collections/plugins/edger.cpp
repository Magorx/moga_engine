#include "redactor/plugin_std.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

static PPluginStatus init(const PAppInterface* appInterface);
static PPluginStatus deinit();

static void dump();
static void onUpdate(double elapsedTime);

static const PPluginInfo  *getInfo();
static PPreviewLayerPolicy getFlushPolicy();

static void onMousePressed(PVec2f mousePos);
static void onMouseMove(PVec2f mouseOldPos, PVec2f mouseNewPos);
static void onMouseReleased(PVec2f mousePos);

static bool enableExtension(const char *name);
static void *getExtensionFunc(const char *name);

static void draw(PVec2f mousePos);


const PPluginInterface gPluginInterface =
{
    0, // std_version
    0, // reserved
    
    enableExtension,
    getExtensionFunc,

    // general
    getInfo,
    init,
    deinit,
    dump,
    onUpdate,
    nullptr,
    getFlushPolicy,

    // effect
    nullptr,

    // tool
    onMousePressed,
    onMouseReleased,
    onMouseMove
};

const PPluginInfo gPluginInfo =
{
    0, // std_version
    0, // reserved

    &gPluginInterface,

    "Edger",
    "1.0",
    "KCTF",
    "Spawns random triangles in a circle, blends them on active layer",
    
    PPT_TOOL
};


const PAppInterface *gAppInterface = nullptr;


extern "C" const PPluginInterface *get_plugin_interface()
{
    return &gPluginInterface;
}

static PPluginStatus init(const PAppInterface* appInterface)
{
    srand(time(NULL));

    gAppInterface = appInterface;
    appInterface->general.log("Edger: succesful initialization!");
    return PPS_OK; 
}

static PPluginStatus deinit()
{
    return PPS_OK;
}

static void dump()
{
}

static const PPluginInfo *getInfo()
{
    return &gPluginInfo;
}

static void onUpdate(double elapsedTime)
{
}

static PPreviewLayerPolicy getFlushPolicy()
{
    return PPLP_BLEND;
}

static void onMousePressed(PVec2f mousePos)
{
    draw(mousePos);
}

static void onMouseMove(PVec2f mouseOldPos, PVec2f mouseNewPos)
{
    draw(mouseNewPos);
}

static void onMouseReleased(PVec2f mousePos)
{
}

static bool enableExtension(const char *name)
{
    return false;
}

static void *getExtensionFunc(const char *name)
{
    return nullptr;
}

static void draw(PVec2f mousePos)
{
    float size = gAppInterface->general.get_size();

    float a1 = rand();
    float a2 = rand();

    PVec2f p0 = mousePos;

    PVec2f p1 = {(float) (mousePos.x + cos(a1) * size), (float) (mousePos.y + sin(a2) * size)};
    PVec2f p2 = {(float) (mousePos.x + cos(a2) * size), (float) (mousePos.y + sin(a1) * size)};

    PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_ACTIVE, nullptr };
    gAppInterface->render.triangle( p0, p1, p2,
                                    gAppInterface->general.get_color(),
                                    &render_mode);
}
