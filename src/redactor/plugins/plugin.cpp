#include <dlfcn.h>

#include "plugin.h"
#include "utils/logger.h"

typedef const PPluginInterface* (*plugin_init_func)();

RedactorPlugin::RedactorPlugin(const char *fileName, const PAppInterface *appInterface) {
    lib_handle = dlopen(fileName, RTLD_NOW);
    if (lib_handle == nullptr) {
        logger.error("Plugin", "can't load plugin named [%s]", fileName);
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

    if (dlclose(lib_handle) != 0) {
        logger.error("~Plugin", "can't close plugin: %s", dlerror());
    }
}
