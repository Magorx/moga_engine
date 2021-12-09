#include "shader_factory.h"


P::Shader *ShaderFactory::compile(const char *code, P::ShaderType type_, bool is_code) const {
    auto type = type_ == P::VERTEX ? RShader::Vertex : type_ == P::FRAGMENT ? RShader::Fragment : RShader::Geometry;
    if (type == RShader::Geometry) {
        return nullptr;
    }

    auto ret = new PluginShader(code, type, is_code);
    if (!ret->is_ok()) {
        delete ret;
        return nullptr;
    }

    return ret;
}

void ShaderFactory::release(P::Shader *shader) const {
    delete dynamic_cast<PluginShader*>(shader);
}

