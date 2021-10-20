#pragma once


#include "color/color.h"


struct Style {
    RGBA color_back;
    RGBA color_front;

    RGBA color_font;
    int char_size;

    RGBA color_outline;
    const char *font_filename;
};
