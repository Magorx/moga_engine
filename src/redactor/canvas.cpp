#include "canvas.h"


Canvas::Canvas(Renderer *renderer, ToolManager *tool_manager, Vec2d size):
renderer(renderer),
tool_manager(tool_manager),
size(size)
{
    if (!renderer || !tool_manager) {
        printf("Canvas was created without renderer or tool_manager, luckily to crush now\n");
    }

    active_layer = new Layer(renderer, size);
    layers.push_back(active_layer);

    draw_layer = new Layer(renderer, size);
    final_layer = new Layer(renderer, size);
    inter_action_layer = new Layer(renderer, size);
}

Canvas::~Canvas() {
    for (auto layer : layers) {
        delete layer;
    }

    delete draw_layer;
    delete inter_action_layer;
}

void Canvas::flush_draw_to_active() {
    auto img = draw_layer->get_texture()->copyToImage();
    img.saveToFile("draw.png");

    draw_layer->flush_to(active_layer, false);
}

void Canvas::flush_to_final() {
    final_layer->clear();
    for (auto layer : layers) {
        if (layer == active_layer) {
            inter_action_layer->clear();
            active_layer->flush_to(inter_action_layer, false, sf::BlendNone);
            draw_layer->flush_to(inter_action_layer, true);
            inter_action_layer->flush_to(final_layer);
        } else {
            layer->flush_to(final_layer);
        }
    }

    // draw_layer->flush_to(final_layer);
    auto img = final_layer->get_texture()->copyToImage();
    img.saveToFile("final.png");
}

void Canvas::new_layer() {
    active_layer = new Layer(renderer, size);
    layers.push_back(active_layer);
}

void Canvas::grab_tool_manager_activity() { tool_manager->set_active_canvas(this); }

void Canvas::on_mouse_down(const Vec2d &pos) {
    tool_manager->on_mouse_down(pos);
}

void Canvas::on_mouse_up(const Vec2d &pos) {
    tool_manager->on_mouse_up(pos);

    // flush_draw_to_active();

    inter_action_layer->flush_to(active_layer, false, sf::BlendNone);

    auto img = active_layer->get_texture()->copyToImage();
    img.saveToFile("active.png");

    draw_layer->clear({0, 0, 0, 0});
    flush_to_final();
}

void Canvas::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    tool_manager->on_mouse_move(from, to);
}
