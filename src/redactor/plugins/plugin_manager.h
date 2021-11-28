#pragma once


#include "plugin.h"

#include <vector>

class ToolManager;
class Tool;
class EffectManager;

class PluginManager {
    std::vector<PPluginInfo*> plugins;
    ToolManager   *tool_manager;
    EffectManager *effect_manager;

public:
    PluginManager(ToolManager *mg_tool, EffectManager *mg_effect);

    PPluginInterface *load(const char filename);
};
