#include "layer.h"


Layer::Layer(Renderer *renderer, Vec2d size, int idx):
Affected(this),
renderer(renderer),
target(nullptr),
final_target(nullptr),
size(size),
idx(idx)
{
    target = new RRendTexture;
    target->create((float) size.x(), (float) size.y());
    target->setRepeated(true);
    target->clear({0, 0, 0, 0});

    final_target = new RRendTexture;
    final_target->create((float) size.x(), (float) size.y());
    final_target->setRepeated(true);
    final_target->clear({0, 0, 0, 0});

    add_effect(new FlushToFinalTexture(this));
}

void Layer::flush_to(Layer *layer, bool to_flip, bool to_apply_effects, RMode rmode) {
    if (!layer || !renderer) return;

    if (to_apply_effects) {
        if (!effects_applied) {
            // final_target->clear({0, 0, 0, 0});
            // renderer->push_target(final_target);
            // renderer->draw_texture({0, 0}, &target->getTexture(), true);
            // renderer->pop_target();

            apply_effects();
            effects_applied = true;
        }

        renderer->push_target(layer->get_target());
        renderer->set_render_state(rmode);
        renderer->draw_texture({0, 0}, &final_target->getTexture(), to_flip);
        renderer->pop_target();
    } else {
        renderer->push_target(layer->get_target());
        renderer->set_render_state(rmode);
        renderer->draw_texture({0, 0}, &target->getTexture(), to_flip);
        renderer->pop_target();
    }
}