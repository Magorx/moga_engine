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

    auto vec1 = new std::vector<RTexture*>;
    vec1->push_back(Resources.texture.button.basic.idle);
    vec1->push_back(Resources.texture.button.basic.hovered);

    auto vec2 = new std::vector<RTexture*>;
    vec2->push_back(Resources.texture.button.basic.idle);
    vec2->push_back(Resources.texture.button.basic.hovered);

    auto anima1 = new AppearenceAnimation(vec1, 0.5, false);
    auto anima2 = new AppearenceAnimation(vec2, 0.5, false);

    auto style = new MouseReactionStyle{
        anima2,
        new AppearenceTexture(Resources.texture.button.basic.pressed),
        anima1,
        anima2
    };

    moga.add_tickable(anima1);
    moga.add_tickable(anima2);

    auto new_canvas_button = new v_Button({0, 0}, style);
    new_canvas_button->e_mouse_release.add(new AddNewCanvasReaction(&moga));

    auto font_color = new SmartColor({47, 47, 47});
    moga.resman.add(font_color);

    new_canvas_button->add_label("NEW", 15, font_color);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    // ==================================================================================

    // ==================================================================================

    // ==================================================================================

    opt_panel->add_spaceholder(10);

}