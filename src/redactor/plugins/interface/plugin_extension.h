#pragma once


#include "redactor/extensions/extension.h"
#include "redactor/plugins/plugin.h"


struct PlugingExtension : public RedactorExtension {
    const RedactorPlugin *plugin;
    
    PlugingExtension(const char *name, uint32_t version, const RedactorPlugin *plugin) :
    RedactorExtension(name, version),
    plugin(plugin)
    {}

    virtual void *get_func(const char *func) override {
        if (!func || !plugin) return nullptr;

        return plugin->interface->ext_get_func(name, func);
    }

    virtual void *get_interface(const char *name) override {
        if (!name || !plugin) return nullptr;
        return plugin->interface->ext_get_interface(this->name, name);
    }
};
