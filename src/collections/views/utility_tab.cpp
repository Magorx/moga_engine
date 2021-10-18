#include "utility_tab.h"

const Color v_UtilityTab::close_button_color = {196, 52, 45};
const Color v_UtilityTab::hide_button_color  = {144, 174, 186};
const Color v_UtilityTab::drag_tab_color     = {240, 30, 30};


v_UtilityTab::v_UtilityTab(Vec2d size, SmartColor *main_color) :
v_Highlighter({{0, 0}, size}, main_color),
buttons_layout(new v_HorizontalLayout(ViewBody{{size.x() - 2 * size.y(), 0}, {2 * size.y(), size.y()}})),
b_close(new v_Button({0, 0}, new SmartColor(close_button_color))),
b_hide(new v_Button({0, 0}, new SmartColor(hide_button_color))),
v_drag_pod(new v_DragPad({0, {size.x() - 2 * size.y(), size.y()}}, main_color)),
v_options(v_Stretcher::X(size.y()))
{
    b_close->e_close.emit({});
    b_hide->e_toggle_activity.emit({Event::Activator::off});
    buttons_layout->layout_add(b_hide);
    buttons_layout->layout_add(b_close);

    add_subview(buttons_layout);
    add_subview(v_drag_pod);

    // v_drag_pod->e_mouse_drag.add(new AVDragAcceptor(this));
}
