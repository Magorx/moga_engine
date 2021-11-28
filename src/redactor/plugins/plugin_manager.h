#pragma once


#include "plugin.h"

#include <vector>


class ToolManager;
class Tool;
class EffectManager;

class PluginManager {
    std::vector<RedactorPlugin*> plugins;
    ToolManager   *tool_manager;
    EffectManager *effect_manager;

public:
    PluginManager(ToolManager *mg_tool, EffectManager *mg_effect);
    ~PluginManager();

    RedactorPlugin *load(const char *filename, PAppInterface *app_interface);
    void fit_plugin(RedactorPlugin *plugin);
};
