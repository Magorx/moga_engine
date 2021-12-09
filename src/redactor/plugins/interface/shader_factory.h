#pragma once


#include "redactor/plugin_std/std.hpp"


class ShaderFactory : public P::ShaderFactory {
    virtual P::Shader *compile(const char *code, P::ShaderType type) const override;
    virtual void       release(P::Shader *)                          const override;
};
