#pragma once


#include "redactor/plugin_std/std.hpp"
#include "visual/renderer/gate_type_aliases.h"
#include "plugin_shader.h"


class ShaderFactory : public PUPPY::ShaderFactory {
    virtual PUPPY::Shader *compile(const char *code, PUPPY::ShaderType type, bool is_code = true) const override;
};
