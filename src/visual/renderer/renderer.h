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

    inline void clear() { scr.window->clear(); }
    inline void display() {scr.window->display();}

    inline sf::RenderWindow *get_window() const {
        return scr.window;
    }

    inline void draw_circle(const Vec3d &pos, const double rad, const Color &color) {
        sf::CircleShape circle(rad);

        RGBA rgba = to_rgba(color);
        circle.setFillColor(sf::Color(rgba.r, rgba.g, rgba.b, rgba.a));
        circle.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
        circle.setOrigin({static_cast<float>(rad), static_cast<float>(rad)});

        scr.window->draw(circle);
    }

    inline void draw_line(const Vec3d &p1, const Vec3d &p2, const Color &color) {
        RGBA rgba = to_rgba(color);
        sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

        sf::Vertex buffer[2] = {
            sf::Vertex({static_cast<float>(p1.x()), static_cast<float>(p1.y())}, sf_color),
            sf::Vertex({static_cast<float>(p2.x()), static_cast<float>(p2.y())}, sf_color),
        };

        scr.window->draw(buffer, 2, sf::Lines);
    }

    inline void draw_square(const Vec3d &pos, const double size, const Color &color) {
        RGBA rgba = to_rgba(color);
        sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

        sf::RectangleShape rect({static_cast<float>(2 * size), static_cast<float>(2 * size)});
        rect.setOrigin({static_cast<float>(size), static_cast<float>(size)});
        rect.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
        rect.setFillColor(sf_color);

        scr.window->draw(rect);
    }
};

#endif // RENDERER_H