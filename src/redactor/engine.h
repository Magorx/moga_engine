#pragma once


#include "MogaEngine.h"

#include "tools/tool_manager.h"
#include "tools/tools.h"

#include "effect_manager.h"

#include "redactor/plugins/interface/app_plugin_interface.h"
#include "plugins/plugin_manager.h"


class RedactorEngine : public MogaEngine {
    ToolManager   *tool_manager;
    EffectManager *effect_manager;
    PluginManager *plugin_manager;

public:
    RedactorEngine(RWindow *window,
                   const char *name);
    
    ~RedactorEngine();
    
    inline ToolManager   *get_tool_manager()   { return tool_manager; }
    inline EffectManager *get_effect_manager() { return effect_manager; }
    inline PluginManager *get_plugin_manager() { return plugin_manager; }

    inline RedactorPluginInterface *get_plugin_interface();

    bool load_plugin(const char *path);
    bool load_plugin(const char *path, bool directory);
};
