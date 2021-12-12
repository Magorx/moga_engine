#pragma once


#include "redactor/plugin_std/std.hpp"

#include "redactor/plugins/interface/target.h"
#include "redactor/plugins/interface/widget_factory.h"
#include "redactor/plugins/interface/shader_factory.h"
#include "redactor/plugins/interface/target_factory.h"


class RedactorPluginInterface : public P::AppInterface {

public:
    RedactorPluginInterface();
    ~RedactorPluginInterface();

// extension
    virtual bool  ext_enable        (const char *name)                        const override;
    virtual void *ext_get_func      (const char *extension, const char *func) const override;
    virtual void *ext_get_interface (const char *extension, const char *name) const override;

// general
    virtual void log(const char *fmt, ...) const override;
    virtual double get_absolute_time()     const override;

    virtual P::RGBA get_color() const override;
    virtual float get_size()    const override;

// target
    virtual P::RenderTarget *get_target()  const override;
    virtual P::RenderTarget *get_preview() const override;
    virtual void flush_preview()           const override;

    virtual void set_color(P::RGBA color) const override;
    virtual void set_size(float size) const override;
};
