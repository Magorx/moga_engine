#include "renderer.h"


Renderer::Renderer(const char *window_name, int size_x, int size_y):
scr(new sf::RenderWindow(sf::VideoMode(size_x, size_y), window_name), size_x, size_y),
offset(0, 0, 0)
{}

Renderer::~Renderer() {
    delete get_window();
}

void Renderer::draw_circle(Vec3d pos, const double rad, const Color &color) {
    pos += offset;

    sf::CircleShape circle(rad);

    RGBA rgba = to_rgba(color);
    circle.setFillColor(sf::Color(rgba.r, rgba.g, rgba.b, rgba.a));
    circle.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    circle.setOrigin({static_cast<float>(rad), static_cast<float>(rad)});

    scr.window->draw(circle);
}

void Renderer::draw_line(Vec3d p1, Vec3d p2, const Color &color) {
    p1 += offset;
    p2 += offset;

    RGBA rgba = to_rgba(color);
    sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

    sf::Vertex buffer[2] = {
        sf::Vertex({static_cast<float>(p1.x()), static_cast<float>(p1.y())}, sf_color),
        sf::Vertex({static_cast<float>(p2.x()), static_cast<float>(p2.y())}, sf_color),
    };

    scr.window->draw(buffer, 2, sf::Lines);
}

void Renderer::draw_square(Vec3d pos, const double size, const Color &color) {
    pos += offset;

    RGBA rgba = to_rgba(color);
    sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

    sf::RectangleShape rect({static_cast<float>(2 * size), static_cast<float>(2 * size)});
    rect.setOrigin({static_cast<float>(size), static_cast<float>(size)});
    rect.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    rect.setFillColor(sf_color);

    scr.window->draw(rect);
}

void Renderer::draw_rectangle(Vec3d pos, const Vec2d size, const Color &color) {
    pos += offset;

    RGBA rgba = to_rgba(color);
    sf::Color sf_color(rgba.r, rgba.g, rgba.b, rgba.a);

    sf::RectangleShape rect({static_cast<float>(size.x()), static_cast<float>(size.y())});
    rect.setPosition({static_cast<float>(-size.x() / 2), static_cast<float>(-size.y() / 2)});
    rect.setOrigin({static_cast<float>(-pos.x() - size.x() / 2), static_cast<float>(-pos.y() - size.y() / 2)});
    rect.setFillColor(sf_color);

    scr.window->draw(rect);
}

void Renderer::draw_text(const char *lable, int char_size, Vec2d pos, const Color &font_color, const Color &back_color, bool to_background, bool to_centrize) {
    pos += (Vec2d) offset;

    RGBA rgba_back = to_rgba(back_color);
    RGBA rgba_font = to_rgba(font_color);
    sf::Color sf_color_back(rgba_back.r, rgba_back.g, rgba_back.b, rgba_back.a);
    sf::Color sf_color_font(rgba_font.r, rgba_font.g, rgba_font.b, rgba_font.a);

    sf::Text text;
    sf::Font font;

    if (!font.loadFromFile("arial.ttf")) {
        printf("sad story\n");
    }

    text.setFont(font);
    text.setString(lable);
    text.setCharacterSize(char_size);

    text.setFillColor(sf_color_font);
    // text.setOutlineColor(sf_color_font);
    // text.setOutlineThickness(1);

    sf::Vector2f sf_pos(static_cast<float>(pos.x()), static_cast<float>(pos.y()));

    text.setPosition(sf_pos);
    sf::Vector2f bounds(text.getLocalBounds().width, text.getLocalBounds().height);

    if (to_centrize) {
        bounds.x /= 2;
        bounds.y /= 2;
        text.setPosition(text.getPosition() - bounds);
        bounds.x *= 2;
        bounds.y *= 2;
    }

    if (to_background) {
        bounds.y *= 2;
        sf::RectangleShape background(bounds);
        background.setFillColor(sf_color_back);
        background.setPosition(text.getPosition());
        scr.window->draw(background);
    }

    scr.window->draw(text);
}
