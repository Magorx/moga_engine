#pragma once


#include "menu.h"


class v_Submenu : public v_Highlighter {
    v_Menu *menu;
public:
    enum Type {
        on_click,
        on_hover,
    };

    v_Submenu(Type type, const ViewBody &body, SmartColor *color, const char *lable, std::vector<const char*> button_lables, SmartColor *button_color, SmartColor *text_color, SmartColor *back_color = nullptr, int char_size = 15, Vec2d button_scale = {1.3, 1.3});

    virtual void refit() override;

    AbstractView *operator[](size_t i);
};
