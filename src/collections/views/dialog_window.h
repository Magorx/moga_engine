#pragma once


#include "window.h"
#include "text_field.h"
#include "stretcher.h"

#include "collections/styles/window.h"
#include "collections/styles/button.h"


const double PX_DIALOG_FIELD_HEIGHT = 32;
const Vec2d PX_BUTTON_SIZE_PADDING = {3, 0};


class v_DialogWindow : public v_Window {
    v_Stretcher *v_stretcher;
    double center_offset;
    double padding;

public:
    v_DialogWindow(const char *name, double width, double padding, double center_offset = 0, WindowStyle *style = Resources.add_style(StdStyle::Window::dialog()));

    v_TextField *add_field(const char *field_name, double max_ans_length = 100, const char *after_name = nullptr);

    v_Button *add_text_button(const char *button_name, 
                              MouseReactionStyle *button_style = Resources.add_style(StdStyle::Button::blue()),
                              TextStyle *text_style = Resources.add_style(StdStyle::Text::basic()));

    v_Button *add_accept_button (const char *button_name, TextStyle *style = Resources.add_style(StdStyle::Text::basic()->negative()));
    v_Button *add_decline_button(const char *button_name, TextStyle *style = Resources.add_style(StdStyle::Text::basic()->negative()));

    void fit_content_to_fields();
};
