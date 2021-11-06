#include "rgb_splined.h"


const int RGB_MAPPING_CNT = 3;


eff_RGBSplined::eff_RGBSplined(Layer *layer) :
ShaderEffect(layer, Resources.shader.name.rgb_mapping)
{
    for (int i = 0; i < RGB_MAPPING_CNT; ++i) {
        mapping[i].resize(255);
        for (size_t j = 0; j < mapping[i].size(); ++j) {
            mapping[i][j] = (double) j / mapping[i].size();
        }
    }

    shader = Resources.create_frag_shader(Resources.shader.name.rgb_mapping);

    shader->setUniformArray("red",   &mapping[0][0], mapping[0].size());
    shader->setUniformArray("green", &mapping[1][0], mapping[1].size());
    shader->setUniformArray("blue",  &mapping[2][0], mapping[2].size());
    shader->setUniform("max_rgb", 254.0f);
}

void eff_RGBSplined::set_spline(int idx, AbstractView *view) {
    if (idx < 0 || idx > RGB_MAPPING_CNT || !view) return;

    view->e_vec_fraction_changed.add(new RGBMappingUpdate(this, idx));
}

void eff_RGBSplined::apply() {
    shader->setUniformArray("red",   &mapping[0][0], mapping[0].size());
    shader->setUniformArray("green", &mapping[1][0], mapping[1].size());
    shader->setUniformArray("blue",  &mapping[2][0], mapping[2].size());
    shader->setUniform("max_rgb", 254.0f);

    ShaderEffect::apply();
}


RGBMappingUpdate::RGBMappingUpdate(eff_RGBSplined *target, int idx) :
EventAcceptor(target),
idx(idx)
{
    if (idx >= RGB_MAPPING_CNT) {
        idx = RGB_MAPPING_CNT - 1;
    }
}

EventAccResult RGBMappingUpdate::operator()(const Event::VectorFractionChanged &event, const EventAccResult *) {
    for (size_t i = 0; i < event.data.size(); ++i) {
        float res = event.data[i] / event.max_coef;
        double map_idx = (double) i / event.data.size() * acceptor->mapping[0].size();
        acceptor->mapping[idx][(size_t) map_idx] = res;
    }

    acceptor->target->force_redraw();
    return EventAccResult::cont;
}
