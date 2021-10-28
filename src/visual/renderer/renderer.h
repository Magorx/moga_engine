#ifndef RENDERER_H
#define RENDERER_H


#include <SFML/Graphics.hpp>
#include <cstring>

#include "utils/vec3d.h"
#include "utils/vec2d.h"
#include "visual/color/color.h"

#include "appearence/appearence.h"

#include "engine/Resources.h"


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


struct RendererState {
    Vec2d offset;
    Appearence *appearence;
    sf::RenderTarget *target;
};


class Renderer {
    Screen scr;

    std::vector<RVertex> cur_verticies;

    std::vector<RendererState> states;
    RendererState *state;

public:
    Renderer(const char *window_name, int size_x, int size_y);
    ~Renderer();

    inline void clear(sf::Color color = sf::Color(0, 0, 0, 255)) { scr.window->clear(color); }
    inline void display() { scr.window->display(); }

    inline sf::RenderWindow *get_window() const {
        return scr.window;
    }

    inline void shift(const Vec2d &delta) {
        state->offset += delta;
    }

    void draw_circle(Vec2d pos, const double rad, const RGBA &color);
    void draw_line(Vec2d p1, Vec2d p2, const RGBA &color);
    void draw_square(Vec2d pos, const double size, const RGBA &color);
    void draw_rectangle(Vec2d pos, const Vec2d size, const RGBA &color);
    void draw_text(const char *label, int size, Vec2d pos, const RGBA &back_color, const RGBA &font_color,  bool to_background, bool to_centrize = false, const RFont *font = Resources.font.basic);

    void apr_draw_circle(Vec2d pos, double rad, int granularity);
    void apr_draw_rectangle(Vec2d pos, const Vec2d size);

    void draw_texture(Vec2d pos, RTexture *texture);

    inline void set_appearence(Appearence *appearence_) { state->appearence = appearence_; }

    static Vec2d get_text_size(const char *text, int char_size, const RFont *font = Resources.font.basic);

    void push_target(sf::RenderTarget *target) {
        if (state) {
            states.push_back({{0, 0}, state->appearence, target});
        } else {
            states.push_back({{0, 0}, nullptr, target});
        }

        state = &states[states.size() - 1];
    }

    void pop_target() {
        if (states.size() <= 1) {
            return;
        }

        states.pop_back();
        state = &states[states.size() - 1];
    }
};

#endif // RENDERER_H