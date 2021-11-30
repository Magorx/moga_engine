#include "layer_permanent.h"
#include "redactor/plugins/plugin.h"

PluginEffect::PluginEffect(RedactorPlugin *plugin) :
Effect(nullptr),
plugin(plugin)
{}

const char *PluginEffect::get_name() const {
    if (!plugin) return "NOPLUGIN"; 
    else return plugin->get_lib()->name;
}

void PluginEffect::apply() {
    plugin->get_inteface()->effect.apply();
}

void PluginEffect::open_settings() {
    if (!plugin) return;
    auto settings = plugin->get_settings();
    if (!settings) return;

    settings->e_toggle_activity.emit({});
}
