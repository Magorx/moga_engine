#include "shader_factory.h"


PluginShader::PluginShader(const char *code, RShader::Type type, bool is_code) {
    if (is_code) {
        if (rshader.loadFromMemory(code, type)) {
            status = 0;
        }
    } else {
        if (rshader.loadFromFile(code, type)) {
            status = 0;
        }
    }
}

void PluginShader::shader_set_uniform_int(const char *name, int  val) {
    rshader.setUniform(name, val);
}

void PluginShader::shader_set_uniform_int_arr(const char *name, int *val, size_t cnt) {
    for (size_t i = 0; i < cnt; ++i) {
        ((float*)val)[i] = (float) val[i];
    }

    rshader.setUniformArray(name, (float*) val, cnt);

    for (size_t i = 0; i < cnt; ++i) {
        val[i] = (int) ((float*)val)[i];
    }
}


void PluginShader::shader_set_uniform_float(const char *name, float  val) {
    rshader.setUniform(name, val);
}

void PluginShader::shader_set_uniform_float_arr(const char *name, float *val, size_t cnt) {
    rshader.setUniformArray(name, val, cnt);
}

bool PluginShader::is_ok() {
    return status == 0;
}

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

