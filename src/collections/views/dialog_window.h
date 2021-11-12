#pragma once


#include "window.h"
#include "collections/styles/window.h"


class v_DialogWindow : public v_Window {
public:
    v_DialogWindow(const char *name, const Vec2d &size, WindowStyle *style = StdStyle::Window::dialog());
};
