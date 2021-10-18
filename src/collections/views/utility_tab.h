#pragma once


#include "mouse_catcher.h"
#include "button.h"
#include "horizontal_layout.h"
#include "stretcher.h"
#include "drag_tab.h"

class v_UtilityTab : public v_Highlighter {
    v_HorizontalLayout *buttons_layout;

    v_Button *b_close;
    v_Button *b_hide;
    v_DragPad *v_drag_pod;
    v_Stretcher *v_options;

public:

    static const Color close_button_color;
    static const Color hide_button_color;
    static const Color drag_tab_color;

    v_UtilityTab(Vec2d size, SmartColor *main_color);

    inline v_DragPad *get_drag_pod() { return v_drag_pod; }

};
