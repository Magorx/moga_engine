#include "layer.h"


Layer::Layer(Renderer *renderer, Vec2d size, int idx):
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
}

void Layer::flush_to(Layer *layer, bool to_flip, RMode rmode) {
    if (!layer || !renderer) return;

    final_target->clear({0, 0, 0, 0});
    renderer->push_target(final_target);
    renderer->draw_texture({0, 0}, &target->getTexture(), true);
    renderer->pop_target();

    apply_effects();

    renderer->push_target(layer->get_target());
    renderer->set_render_state(rmode);
    renderer->draw_texture({0, 0}, &final_target->getTexture(), to_flip);
    renderer->pop_target();
}