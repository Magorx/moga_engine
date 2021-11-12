#pragma once


#include "window.h"
#include "text_field.h"
#include "stretcher.h"
#include "collections/styles/window.h"


const double PX_DIALOG_FIELD_HEIGHT = 32;


class v_DialogWindow : public v_Window {
    v_Stretcher *v_stretcher;
    double center_offset;

public:
    v_DialogWindow(const char *name, const Vec2d &size, double center_offset = 0, WindowStyle *style = StdStyle::Window::dialog());

    v_TextField *add_field(const char *field_name, double max_ans_length = 100, const char *after_name = nullptr);
};
