#include "renderer.h"


Renderer::Renderer(const char *window_name, int size_x, int size_y):
scr(new sf::RenderWindow(sf::VideoMode(size_x, size_y), window_name), size_x, size_y)
{}

Renderer::~Renderer() {
    delete get_window();
}

void Renderer::draw_circle(const Vec3d &pos, const double rad, const Color &color) {
    sf::CircleShape circle(rad);

    RGBA rgba = to_rgba(color);
    circle.setFillColor(sf::Color(rgba.r, rgba.g, rgba.b, rgba.a));
    circle.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    circle.setOrigin({static_cast<float>(rad), static_cast<float>(rad)});

    scr.window->draw(circle);
}

void Renderer::draw_line(const Vec3d &p1, const Vec3d &p2, const Color &color) {
    RGBA rgba = to_rgba(color);
    sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

    sf::Vertex buffer[2] = {
        sf::Vertex({static_cast<float>(p1.x()), static_cast<float>(p1.y())}, sf_color),
        sf::Vertex({static_cast<float>(p2.x()), static_cast<float>(p2.y())}, sf_color),
    };

    scr.window->draw(buffer, 2, sf::Lines);
}

void Renderer::draw_square(const Vec3d &pos, const double size, const Color &color) {
    RGBA rgba = to_rgba(color);
    sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

    sf::RectangleShape rect({static_cast<float>(2 * size), static_cast<float>(2 * size)});
    rect.setOrigin({static_cast<float>(size), static_cast<float>(size)});
    rect.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    rect.setFillColor(sf_color);

    scr.window->draw(rect);
}

void Renderer::draw_rectangle(const Vec3d &pos, const Vec2d size, const Color &color) {
    RGBA rgba = to_rgba(color);
    sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

    sf::RectangleShape rect({static_cast<float>(size.x()), static_cast<float>(size.y())});
    rect.setPosition({static_cast<float>(-size.x() / 2), static_cast<float>(-size.y() / 2)});
    rect.setOrigin({static_cast<float>(-pos.x() - size.x() / 2), static_cast<float>(-pos.y() - size.y() / 2)});
    rect.setFillColor(sf_color);

    scr.window->draw(rect);
}
