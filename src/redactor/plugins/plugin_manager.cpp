#include "plugin_manager.h"


PluginManager::PluginManager(ToolManager *mg_tool, EffectManager *mg_effect) : 
tool_manager(mg_tool),
effect_manager(mg_effect)
{}

PPluginInterface *PluginManager::load(const char filename) {
    return nullptr;
}