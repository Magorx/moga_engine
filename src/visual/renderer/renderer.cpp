#include "renderer.h"


Renderer::Renderer(const char *window_name, int size_x, int size_y):
scr(new sf::RenderWindow(sf::VideoMode(size_x, size_y), window_name), size_x, size_y),
offset(0, 0, 0),
cur_font_filename(nullptr)
{}

Renderer::~Renderer() {
    if (cur_font_filename) {
        free(cur_font_filename);
    }
    delete get_window();
}

void Renderer::draw_circle(Vec3d pos, const double rad, const RGBA &color, sf::RenderTarget *texture) {
    pos += offset;

    sf::CircleShape circle(rad);

    circle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    circle.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    circle.setOrigin({static_cast<float>(rad), static_cast<float>(rad)});

    if (!texture) scr.window->draw(circle);
    else texture->draw(circle);
}

void Renderer::draw_line(Vec3d p1, Vec3d p2, const RGBA &color, sf::RenderTarget *texture) {
    p1 += offset;
    p2 += offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);

    sf::Vertex buffer[2] = {
        sf::Vertex({static_cast<float>(p1.x()), static_cast<float>(p1.y())}, sf_color),
        sf::Vertex({static_cast<float>(p2.x()), static_cast<float>(p2.y())}, sf_color),
    };

    if (!texture) scr.window->draw(buffer, 2, sf::Lines);
    else texture->draw(buffer, 2, sf::Lines);
}

void Renderer::draw_square(Vec3d pos, const double size, const RGBA &color, sf::RenderTarget *texture) {
    pos += offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);

    sf::RectangleShape rect({static_cast<float>(2 * size), static_cast<float>(2 * size)});
    rect.setOrigin({static_cast<float>(size), static_cast<float>(size)});
    rect.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    rect.setFillColor(sf_color);

    if (!texture) scr.window->draw(rect);
    else texture->draw(rect);
}

void Renderer::draw_rectangle(Vec3d pos, const Vec2d size, const RGBA &color, sf::RenderTarget *texture) {
    pos += offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);

    sf::RectangleShape rect({static_cast<float>(size.x()), static_cast<float>(size.y())});
    rect.setPosition({static_cast<float>(-size.x() / 2), static_cast<float>(-size.y() / 2)});
    rect.setOrigin({static_cast<float>(-pos.x() - size.x() / 2), static_cast<float>(-pos.y() - size.y() / 2)});
    rect.setFillColor(sf_color);

    if (!texture) scr.window->draw(rect);
    else texture->draw(rect);
}

void Renderer::draw_text(const char *label, int char_size, Vec2d pos, const RGBA &font_color, const RGBA &back_color, bool to_background, bool to_centrize,  const char *font_filename, sf::RenderTarget *texture) {
    pos += (Vec2d) offset;

    sf::Color sf_color_back(back_color.r, back_color.g, back_color.b, back_color.a);
    sf::Color sf_color_font(font_color.r, font_color.g, font_color.b, font_color.a);

    sf::Text text;
    load_font(cur_font, font_filename, &cur_font_filename);

    text.setFont(cur_font);
    text.setString(label);
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
        if (!texture) scr.window->draw(background);
        else texture->draw(background);
    }

    if (!texture) scr.window->draw(text);
    else texture->draw(text);
}

void Renderer::load_font(sf::Font &font_holder, const char *font_filename, char **cur_font_filename) {
    if (!font_filename) return;
    if (cur_font_filename && *cur_font_filename && !strcmp(font_filename, *cur_font_filename)) return;

    if (!font_holder.loadFromFile(font_filename)) {
        printf("sad story, can't load font [%s]\n", font_filename);
    } else {
        if (cur_font_filename) {
            if (*cur_font_filename) free(*cur_font_filename);
            *cur_font_filename = strdup(font_filename);
        }
    }
}

Vec2d Renderer::get_text_size(const char *text, int char_size, const char *font_filename) {
    sf::Text sf_text = {};
    sf::Font font = {};
    load_font(font, font_filename);

    sf_text.setFont(font);
    sf_text.setString(text);
    sf_text.setCharacterSize(char_size);

    sf::Vector2f bounds(sf_text.getLocalBounds().width, sf_text.getLocalBounds().height);
    return {bounds.x, bounds.y * 2};
}
