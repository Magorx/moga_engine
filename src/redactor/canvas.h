#pragma once


#include <vector>

#include "layer.h"


class Canvas {
    std::vector<Layer*> layers;
    Layer *active_layer;

    Layer *draw_layer;

public:
    Canvas(Vec2d size) {
        active_layer = new Layer(size);
        layers.push_back(active_layer);

        draw_layer = new Layer(size);
    }

    ~Canvas() {
        for (auto layer : layers) {
            delete layer;
        }

        delete draw_layer;
    }

    Layer *get_active_layer() { return active_layer; }

    Layer *get_draw_layer() { return draw_layer; }

};
