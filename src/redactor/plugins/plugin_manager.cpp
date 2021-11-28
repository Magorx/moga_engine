#include "plugin_manager.h"
#include "utils/logger.h"

#include "redactor/engine.h"

#include "redactor/tools/plugin_tool.h"


PluginManager::PluginManager(ToolManager *mg_tool, EffectManager *mg_effect) : 
tool_manager(mg_tool),
effect_manager(mg_effect)
{}

PluginManager::~PluginManager() {
    for (auto plugin : plugins) {
        delete plugin;
    }
}

RedactorPlugin *PluginManager::load(const char *filename, PAppInterface *app_interface) {
    if (!filename || !app_interface) {
        logger.error("PluginManager", "bad params for plugin load: filename[%s], app_interface[%p]", filename, app_interface);
    }

    RedactorPlugin *plugin = new RedactorPlugin(filename, app_interface);
    if (!plugin || !plugin->is_ok()) {
        logger.error("PluginManager", "plugin[%p] with filename[%s] was technicaly loaded, but somewhy is not ok", plugin, filename);
        delete plugin;
        return nullptr;
    }

    plugins.push_back(plugin);

    fit_plugin(plugin);

    return plugin;
}

void PluginManager::fit_plugin(RedactorPlugin *plugin) {
    if (!plugin) return;

    if (plugin->get_type() == PPluginType::PPT_TOOL) {
        tool_manager->add_tool(new t_Plugin(tool_manager, plugin));
    } else if (plugin->get_type() == PPluginType::PPT_EFFECT) {
        
    }
}
