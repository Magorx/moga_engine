#pragma once


#include "mouse_catcher.h"
#include "button.h"
#include "horizontal_layout.h"
#include "stretcher.h"
#include "drag_tab.h"
#include "toggler.h"

#include "visual/style/utility_bar_style.h"


class v_UtilityTab : public v_Highlighter {
    v_HorizontalLayout *buttons_layout;

    v_Button *b_close;
    v_Button *b_hide;
    v_DragPad *v_drag_pod;
    v_Stretcher *v_options;

    v_Button *v_accessory;

    v_Highlighter *v_l_corner;
    v_Highlighter *v_r_corner;
    v_Highlighter *v_middle;

public:

    static const Color close_button_color;
    static const Color hide_button_color;
    static const Color drag_tab_color;

    v_UtilityTab(Vec2d size, SmartColor *main_color);
    v_UtilityTab(Vec2d size, UtilityBarStyle *style);

    inline v_DragPad *get_drag_pod()    { return v_drag_pod; }
    inline v_Button *get_button_close() { return b_close; }
    inline v_Button *get_button_hide()  { return b_hide; }
    inline AbstractView *get_accessory() { return v_accessory; }

};
