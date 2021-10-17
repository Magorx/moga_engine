#pragma once


#include "vertical_layout.h"
#include "visual/renderer/renderer.h"

#include <vector>


class v_Menu : public v_VerticalLayout {
    std::vector<char*> lables;

    SmartColor *button_color;
    SmartColor *text_color;
    int char_size;
    Vec2d button_scale;

public:
    v_Menu(std::vector<const char*> button_lables, SmartColor *button_color, SmartColor *text_color, SmartColor *back_color = nullptr, int char_size = 15, Vec2d button_scale = {1.3, 1.3});
    virtual ~v_Menu();

    AbstractView *operator[](size_t i);

    virtual void refit() override;

    void recalculate_size();
};
