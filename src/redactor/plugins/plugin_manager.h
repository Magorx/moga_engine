#pragma once


#include "plugin.h"
#include "settings_window.h"

#include <vector>
#include <map>


class ToolManager;
class Tool;
class EffectManager;


struct PluginGlobalInfo {
    PluginSettingsWindow *settings;
    PPluginInfo *info;
};


class PluginManager {
    std::vector<RedactorPlugin*> plugins;
    ToolManager   *tool_manager;
    EffectManager *effect_manager;

    std::map<const PPluginInterface*, RedactorPlugin*> plugin_map;

public:
    PluginManager(ToolManager *mg_tool, EffectManager *mg_effect);
    ~PluginManager();

    RedactorPlugin *load(const char *filename, PAppInterface *app_interface);
    void fit_plugin(RedactorPlugin *plugin);

    void set_plugin(const PPluginInterface *plugin_self, RedactorPlugin *plugin);
    RedactorPlugin *get_plugin(const PPluginInterface *plugin_self);
};
