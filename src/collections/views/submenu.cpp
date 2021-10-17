#include "submenu.h"
#include "Collections.h"


v_Submenu::v_Submenu(Type type, const ViewBody &body_, SmartColor *color, const char *lable, std::vector<const char*> button_lables, SmartColor *button_color, SmartColor *text_color, SmartColor *back_color, int char_size, Vec2d button_scale):
v_Highlighter(body_, color)
{
    add_label(lable, char_size, text_color);

    menu = new v_Menu(button_lables, button_color, text_color, back_color, char_size, button_scale);
    menu->deactivate();
    menu->e_toggle_activity.add(new HideableActivityToggleAcceptor(menu));
    
    if (type == Type::on_hover) {
        e_mouse_move.add(new a_OnHoverToggler(this, menu), false);
    } else {
        e_mouse_press.add(new a_OnPressToggler(this, menu), false);
    }
    
    add_subview(menu);

    // refit();
}

void v_Submenu::refit() {
    v_Highlighter::refit();
    
    menu->set_min_size({body.size.x(), 0});
    menu->fit({0, 1}, {1, 2});
    menu->refit();
}

AbstractView *v_Submenu::operator[](size_t i) {
    return (*menu)[i];
}
