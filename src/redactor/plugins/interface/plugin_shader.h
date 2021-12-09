#pragma once


#include "redactor/plugin_std/std.hpp"
#include "visual/renderer/gate_type_aliases.h"


struct PluginShader : public P::Shader {
    PluginShader(const char *code, RShader::Type type, bool is_code = true);
    virtual ~PluginShader() {}

    virtual void shader_set_uniform_int      (const char *name, int  val) override;
    virtual void shader_set_uniform_int_arr  (const char *name, int *val, size_t cnt) override;

    virtual void shader_set_uniform_float    (const char *name, float  val) override;
    virtual void shader_set_uniform_float_arr(const char *name, float *val, size_t cnt) override;

    bool is_ok();

    const RShader *get_shader() const { return &rshader; }

private:
    RShader rshader;
    int status = -1;
};
