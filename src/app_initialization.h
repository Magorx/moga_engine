#pragma once

const int SCR_W  = 1000;
const int SCR_H  = 600;

#include "chemistry/chem_engine.h"
#include "scene_generation.h"
#include "responses.h"

void initialize_photoshop(ChemEngine &moga) {
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

    auto font_color = new SmartColor({47, 47, 47});
    moga.resman.add(font_color);

    new_canvas_button->add_label("New CANVAS", 25, font_color);

    opt_panel->add_spaceholder(2);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    // new_canvas_button_style = StdStyle::Button::hide();

    // new_canvas_button = new v_Button({50, 32}, new_canvas_button_style);
    // new_canvas_button->e_mouse_release.add(new AddNewCanvasReaction(&moga));

    // moga.resman.add(new_canvas_button_style);

    // font_color = new SmartColor({47, 47, 47});
    // moga.resman.add(font_color);

    // opt_panel->add_spaceholder(2);

    // moga.add_view(new_canvas_button);

    // ==================================================================================

    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("Aboba", {200, 200}, window_style);

    moga.add_view(window);

    // ==================================================================================

    opt_panel->add_spaceholder(2);

}