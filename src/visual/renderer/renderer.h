#ifndef RENDERER_H
#define RENDERER_H


#include <SFML/Graphics.hpp>

#include "utils/vec3d.h"
#include "visual/color/color.h"


struct Screen {
    sf::RenderWindow *window;

    int size_x;
    int size_y;

    inline Screen(sf::RenderWindow *window, int size_x, int size_y):
    window(window),
    size_x(size_x),
    size_y(size_y)
    {}
};


class Renderer {
    Screen scr;

public:
    Renderer(const char *window_name, int size_x, int size_y);

    inline void clear() {scr.window->clear();}
    inline void display() {scr.window->display();}

    inline sf::RenderWindow *get_window() const {
        return scr.window;
    }

    inline void draw_circle(const Vec3d &pos, const double rad, const Color &color) {
        sf::CircleShape circle(rad);

        circle.setFillColor(sf::Color(to_rgba(color).ui32));
        circle.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
        circle.setOrigin({static_cast<float>(rad), static_cast<float>(rad)});

        scr.window->draw(circle);
    }
};

#endif // RENDERER_H