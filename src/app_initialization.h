#pragma once

const int SCR_W  = 1000;
const int SCR_H  = 600;

#include "redactor/engine.h"
#include "scene_generation.h"
#include "responses.h"

void initialize_photoshop(RedactorEngine &moga) {
    SmartColor *color = new SmartColorSin(Color{40, 230, 150});
    moga.add_tickable(color);
    create_cage(&moga, color);
    generate_balls(&moga, 10);

    // ==================================================================================

    auto appr_frame_box = new AppearenceTexture(Resources.texture.frame_gray);
    v_HorizontalLayout *opt_panel = new v_HorizontalLayout({{0, 0}, {SCR_W, 30}}, {{0, 0}, {1, 1}}, 0, nullptr, nullptr);
    opt_panel->set_appearence(appr_frame_box);
    
    moga.add_view(opt_panel);

    // ==================================================================================

    auto new_canvas_button = new v_Button({0, 0}, StdStyle::Button::basic());
    new_canvas_button->e_clicked.add(new AddNewCanvasReaction(&moga));
    new_canvas_button->add_label("Canvas", Resources.font.size.basic_menu, Resources.font.smart_color.basic_menu);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    auto new_picker_button = new v_Button({0, 0}, StdStyle::Button::basic());
    new_picker_button->e_clicked.add(new AddNewColorPickerReaction(&moga));
    new_picker_button->add_label("Color Pucker", Resources.font.size.basic_menu, Resources.font.smart_color.basic_menu);

    opt_panel->layout_add(new_picker_button, 1.5);

    // ==================================================================================

    spawn_canvas_window(&moga, {200, {200, 300}});

    spawn_color_picker_window(&moga, {50, {200, 200}});

    // ==================================================================================

    opt_panel->add_spaceholder(6);

}