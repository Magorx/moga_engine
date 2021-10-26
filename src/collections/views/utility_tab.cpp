#include "utility_tab.h"


const Color v_UtilityTab::close_button_color = {196, 52, 45};
const Color v_UtilityTab::hide_button_color  = {144, 174, 186};
const Color v_UtilityTab::drag_tab_color     = {240, 30, 30};


v_UtilityTab::v_UtilityTab(Vec2d size, SmartColor *main_color) :
v_Highlighter({{0, 0}, size}, main_color, nullptr, 0),
buttons_layout(new v_HorizontalLayout(ViewBody{{size.x() - 2 * size.y(), 0}, {2 * size.y(), size.y()}})),
b_close(new v_Button({0, 0}, new SmartColor(close_button_color))),
b_hide(new v_Button({0, 0}, new SmartColor(hide_button_color))),
v_drag_pod(new v_DragPad({0, {size.x() - 2 * size.y(), size.y()}}, main_color)),
v_options(v_Stretcher::X(size.y()))
{
    buttons_layout->layout_add(b_hide);
    buttons_layout->layout_add(b_close);

    add_subview(buttons_layout);
    add_subview(v_drag_pod);

    b_close->e_mouse_release.add(new AVCloseGenerator(b_close));

    // v_drag_pod->e_mouse_drag.add(new AVDragAcceptor(this));
}


v_UtilityTab::v_UtilityTab(Vec2d size, UtilityBarStyle *style) :
v_Highlighter({{0, 0}, size}, nullptr, nullptr, 0),
buttons_layout(new v_HorizontalLayout(ViewBody{{size.x() - 2 * PX_UTIL_BUTTON_SIZE - PX_UTIL_PADDING, PX_UTIL_PADDING}, {2 * PX_UTIL_BUTTON_SIZE, size.y() - 2 * PX_UTIL_PADDING}})),

b_close(new v_Button({PX_UTIL_PADDING, 0}, style ? style->close_button : nullptr)),
b_hide(new v_Button({PX_UTIL_PADDING, 0}, style ? style->hide_button : nullptr)),

v_drag_pod(new v_DragPad({0, {size.x(), size.y()}})),
v_options(v_Stretcher::X(size.y())),

v_accessory(new v_Button({0, size.y()}, (SmartColor*) nullptr, nullptr, 0)),

v_l_corner(new v_Highlighter({0, size.y()}, nullptr, nullptr, 0)),
v_r_corner(new v_Highlighter({{size.x() - size.y(), 0}, size.y()}, nullptr, nullptr, 0)),
v_middle  (new v_Highlighter({{size.y(), 0}, {size.x() - 2 * size.y(), size.y()}}, nullptr, nullptr, 0))
{
    appearenced = true;

    buttons_layout->layout_add(b_hide);
    buttons_layout->layout_add(b_close);

    add_subview(buttons_layout);
    add_subview(v_drag_pod);
    add_subview(v_accessory);

    add_subview(v_l_corner);
    add_subview(v_r_corner);
    add_subview(v_middle);

    if (style) {
        v_l_corner->set_appearence(style->l_corner);
        v_r_corner->set_appearence(style->r_corner);
        v_middle->set_appearence(style->bar);
    }


    b_close->e_mouse_release.add(new AVCloseGenerator(b_close));
}
