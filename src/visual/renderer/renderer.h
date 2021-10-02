#ifndef RENDERER_H
#define RENDERER_H


#include <SFML/Graphics.hpp>

#include "utils/vec3d.h"
#include "utils/vec2d.h"
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
    ~Renderer();

    inline void clear() { scr.window->clear(); }
    inline void display() {scr.window->display();}

    inline sf::RenderWindow *get_window() const {
        return scr.window;
    }

    void draw_circle(const Vec3d &pos, const double rad, const Color &color);
    void draw_line(const Vec3d &p1, const Vec3d &p2, const Color &color);
    void draw_square(const Vec3d &pos, const double size, const Color &color);
    void draw_rectangle(const Vec3d &pos, const Vec2d size, const Color &color);
};

#endif // RENDERER_H