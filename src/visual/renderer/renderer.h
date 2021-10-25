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


class Renderer {
    Screen scr;
    Vec2d offset;

    sf::Font cur_font;
    char *cur_font_filename;

    Appearence *appearence;
    std::vector<RVertex> cur_verticies;

public:
    Renderer(const char *window_name, int size_x, int size_y);
    ~Renderer();

    inline void clear()   { scr.window->clear(); }
    inline void display() { scr.window->display(); }

    inline sf::RenderWindow *get_window() const {
        return scr.window;
    }

    inline void shift(const Vec2d &delta) {
        offset += delta;
    }

    void draw_circle(Vec2d pos, const double rad, const RGBA &color);
    void draw_line(Vec2d p1, Vec2d p2, const RGBA &color);
    void draw_square(Vec2d pos, const double size, const RGBA &color);
    void draw_rectangle(Vec2d pos, const Vec2d size, const RGBA &color);
    void draw_text(const char *label, int size, Vec2d pos, const RGBA &back_color, const RGBA &font_color,  bool to_background, bool to_centrize = false, const RFont *font = Resources.font.aseprite);

    void apr_draw_circle(Vec2d pos, double rad, int granularity);
    void apr_draw_rectangle(Vec2d pos, const Vec2d size);

    inline void set_appearence(Appearence *appearence_) { appearence = appearence_; }

    static void load_font(sf::Font &font_holder, const char *font_filename, char **cur_font_filename = nullptr);
    static Vec2d get_text_size(const char *text, int char_size, const RFont *font = Resources.font.aseprite);

};

#endif // RENDERER_H