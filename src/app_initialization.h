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

    auto new_canvas_button_style = StdStyle::Button::basic();

    auto new_canvas_button = new v_Button({0, 0}, new_canvas_button_style);
    new_canvas_button->e_mouse_release.add(new AddNewCanvasReaction(&moga));

    auto font_color = new SmartColor(Resources.font.color.basic_menu);
    moga.resman.add(font_color);

    new_canvas_button->add_label("New CANVAS", 25, font_color);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    // ==================================================================================

    auto win = spawn_canvas_window(&moga, 300, 200);

    auto picker = new v_ColorPicker({10, {100, 100}},  moga.get_tool_manager());

    win->add_subview(picker);

    // ==================================================================================

    opt_panel->add_spaceholder(6);

}