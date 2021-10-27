#pragma once


#include <vector>

#include "layer.h"


class Canvas {
    Renderer *renderer;

    std::vector<Layer*> layers;
    Layer *active_layer;

    Layer *draw_layer;
    Layer *final_layer;

public:
    Canvas(Renderer *renderer, Vec2d size):
    renderer(renderer)
    {
        active_layer = new Layer(renderer, size);
        layers.push_back(active_layer);

        draw_layer = new Layer(renderer, size);
        final_layer = new Layer(renderer, size);
    }

    ~Canvas() {
        for (auto layer : layers) {
            delete layer;
        }

        delete draw_layer;
    }

    Layer *get_active_layer() { return active_layer; }
    Layer *get_final_layer() { return final_layer; }
    Layer *get_draw_layer() { return draw_layer; }

    void flush_draw_to_active() {
        draw_layer->flush_to(active_layer);
    }

    void flush_to_final() {
        for (auto layer : layers) {
            layer->flush_to(final_layer);
        }
    }

};
