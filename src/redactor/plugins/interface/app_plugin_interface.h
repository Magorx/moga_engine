#pragma once


#include "redactor/plugin_std/std.hpp"

#include "redactor/plugins/interface/target.h"
#include "redactor/plugins/interface/widget_factory.h"
#include "redactor/plugins/interface/shader_factory.h"
#include "redactor/plugins/interface/target_factory.h"


class RedactorPluginInterface : public P::AppInterface {
// extension
    virtual bool  enable        (std::string_view name)                             const override;
    virtual void *get_func      (std::string_view extension, std::string_view func) const override;
    virtual void *get_interface (const char *extension, const char *name)           const override;

// general
    virtual void log(const char *fmt, ...) const override;
    virtual double get_absolute_time()     const override;

    virtual P::RGBA get_color() const override;
    virtual float get_size()    const override;

// target
    virtual P::RenderTarget *get_target()  const;
    virtual P::RenderTarget *get_preview() const;
    virtual void flush_preview()           const override;
};
