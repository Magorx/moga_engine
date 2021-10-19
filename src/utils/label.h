#pragma once


#include <cstring>
#include "vec3d.h"


struct TextStyle {
    int char_size;
    Vec3d font_color;
    const char *font_filename;

    TextStyle(const TextStyle &other):
    char_size(other.char_size),
    font_color(other.font_color),
    font_filename(other.font_filename ? strdup(other.font_filename) : nullptr)
    {}

    TextStyle &operator=(const TextStyle &other) = delete;
};

struct Lable {
    char *text;
    TextStyle style;

    Lable(const Lable &other):
    text(other.text ? strdup(other.text) : nullptr),
    style(other.style)
    {}

    Lable &operator=(const Lable &other) = delete;

};
