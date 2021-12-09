#pragma once


#include "plugin.h"

#include <vector>
#include <map>


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

    RedactorPlugin *load(const char *filename, P::AppInterface *app_interface);
    void fit_plugin(RedactorPlugin *plugin);

    // void set_plugin(const P::PluginInterface *plugin_self, RedactorPlugin *plugin);
    // RedactorPlugin *get_plugin(const P::PluginInterface *plugin_self);
};
