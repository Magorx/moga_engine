#include <dlfcn.h>

#include "plugin.h"
#include "utils/logger.h"

typedef const PPluginInterface* (*plugin_init_func)();

RedactorPlugin::RedactorPlugin(const char *fileName, const PAppInterface *appInterface) {
    status = -1;

    lib_handle = dlopen(fileName, RTLD_NOW);
    if (lib_handle == nullptr) {
        logger.error("Plugin", "can't dlopen plugin named [%s]: %s", fileName, dlerror());
        return;
    }

    plugin_init_func plugin_init = (plugin_init_func) dlsym(lib_handle, PGET_INTERFACE_FUNC);
    if (plugin_init == nullptr) {
        logger.error("Plugin", "can't get interface of plugin [%s]", fileName);
        dlclose(lib_handle);
        return;
    }
    
    interface = plugin_init();
    if (!interface) {
        logger.error("Plugin", "plugin [%s] returned empty interface", fileName);
        dlclose(lib_handle);
        return;
    }

    PPluginStatus init_status = interface->general.init(appInterface);
    if (init_status != PPS_OK) {
        logger.error("Plugin", "initialization of plugin [%s] failed", fileName);
        dlclose(lib_handle);
        return;
    }
    
    lib = interface->general.get_info();
    if (!lib) {
        logger.error("Plugin", "plugin [%s] returned empty information", fileName);
        dlclose(lib_handle);
        return;
    }

    logger.info("Plugin", "loaded plugin [%s] properly. Author: %s, version: %s (std %lu).\n", lib->name, lib->version, lib->std_version);

    status = 0;
}

RedactorPlugin::~RedactorPlugin() {
    if (status >= 0) {
        if (!interface->general.deinit) {
            logger.error("~Plugin", "plugin [%s] doesn't have deinit() func somewhy, could wasted some resourses", lib->name);
        }

        interface->general.deinit();
        status = -1;
    }

    if (!lib_handle) {
        logger.error("~Plugin", "a plugin doesn't even have dl_handle");
    } else {
        if (dlclose(lib_handle) != 0) {
            logger.error("~Plugin", "can't close plugin: %s", dlerror());
        }
    }
}

void RedactorPlugin::on_mouse_press(const Vec2d &position) {
    interface->tool.on_press(to_pvec2d(position));
}

void RedactorPlugin::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    interface->tool.on_move(to_pvec2d(from), to_pvec2d(to));
}

void RedactorPlugin::on_mouse_release(const Vec2d &position) {
    interface->tool.on_release(to_pvec2d(position));
}

void RedactorPlugin::apply() {
    interface->effect.apply();
}
