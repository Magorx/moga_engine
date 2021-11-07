#include "canvas.h"


Canvas::Canvas(Renderer *renderer, ToolManager *tool_manager, Vec2d size):
renderer(renderer),
tool_manager(tool_manager),
size(size)
{
    if (!renderer || !tool_manager) {
        printf("Canvas was created without renderer or tool_manager, luckily to crush now\n");
    }

    active_layer = new Layer(renderer, this, size);
    layers.push_back(active_layer);

    draw_layer = new Layer(renderer, this, size);
    final_layer = new Layer(renderer, this, size);
    inter_action_layer = new Layer(renderer, this, size);
}

Canvas::~Canvas() {
    for (auto layer : layers) {
        delete layer;
    }

    delete draw_layer;
    delete inter_action_layer;
}

void Canvas::flush_draw_to_active() {
    draw_layer->flush_to(active_layer, false);
}

void Canvas::flush_to_final() {
    final_layer->clear();
    for (auto layer : layers) {
        if (layer == active_layer) {
            inter_action_layer->clear();
            active_layer->flush_to(inter_action_layer, false, false, sf::BlendNone);
            // draw_layer->set_effects(active_layer->get_effects());
            draw_layer->flush_to(inter_action_layer, true, false);

            inter_action_layer->set_effects(active_layer->get_effects());
            inter_action_layer->flush_to(final_layer, false, true);
        } else {
            layer->flush_to(final_layer, true, true);
        }
    }

    _to_redraw = false;
}

int Canvas::new_layer() {
    active_layer = new Layer(renderer, this, size);
    layers.push_back(active_layer);

    flush_to_final();
    return layers.size() - 1;
}

int Canvas::next_layer(int delta) {
    if (!layers.size()) return -1;
    if (!active_layer) {
        active_layer = layers[0];
        flush_to_final();
        return 0;
    }
    
    int cur_idx = idx_by_layer(active_layer);
    if (cur_idx == -1) {
        return -1;
    }

    if (cur_idx + delta < 0 || cur_idx + delta >= (int) layers.size()) {
        return cur_idx;
    }

    active_layer = layers[cur_idx + delta];
    flush_to_final();
    return cur_idx + delta;
}

int Canvas::next_layer() {
    return next_layer(+1);
}

int Canvas::prev_layer() {
    return next_layer(-1);
}

int Canvas::idx_by_layer(Layer *layer) {
    for (size_t i = 0; i < layers.size(); ++i) {
        if (layer == layers[i]) {
            return i;
        }
    }

    return -1;
}

void Canvas::grab_tool_manager_activity() { tool_manager->set_active_canvas(this); }

void Canvas::on_mouse_down(const Vec2d &pos) {
    tool_manager->on_mouse_down(pos);
}

void Canvas::on_mouse_up(const Vec2d &pos) {
    tool_manager->on_mouse_up(pos);

    active_layer->flush_to(inter_action_layer, false, false, sf::BlendNone);
    draw_layer->flush_to(inter_action_layer, true, false);
    inter_action_layer->flush_to(active_layer, false, false, sf::BlendNone);

    draw_layer->clear({0, 0, 0, 0});
    flush_to_final();
}

void Canvas::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    tool_manager->on_mouse_move(from, to);
}

void Canvas::save_to_file(const char *filename) {
    if (!filename) {
        return;
    }

    int len = strlen(filename);
    char *name = (char*) calloc(len + 4, sizeof(char));
    strcpy(name, filename);
    name[len + 0] = '.';
    name[len + 1] = 'p';
    name[len + 2] = 'n';
    name[len + 3] = 'g';

    flush_to_final();
    final_layer->get_texture()->copyToImage().saveToFile(name);

    free(name);
}
