#pragma once

const int SCR_W  = 1200;
const int SCR_H  = 800;

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

    auto new_canvas_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_canvas_button->e_clicked.add(new AddNewCanvasReaction(&moga));
    new_canvas_button->add_label("Canvas", Resources.font.size.basic_menu, Resources.font.color.basic_menu);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    auto new_picker_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_picker_button->e_clicked.add(new AddNewColorPickerReaction(&moga));
    new_picker_button->add_label("Color Picker", Resources.font.size.basic_menu, Resources.font.color.basic_menu);

    opt_panel->layout_add(new_picker_button, 1.5);

    // ==================================================================================

    auto open_img_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    open_img_button->e_clicked.add(new OpenImage(&moga));
    open_img_button->add_label("Open Image", Resources.font.size.basic_menu, Resources.font.color.basic_menu);

    opt_panel->layout_add(open_img_button, 1.5);

    // ==================================================================================
    auto bip_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    bip_button->add_label("Tools", Resources.font.size.basic_menu, Resources.font.color.basic_menu);

    opt_panel->layout_add(bip_button, 1.5);
    bip_button->e_clicked.add(new AddNewToolManagerWindowReaction(&moga));
    // ==================================================================================

    spawn_canvas_window(&moga, {{800, 100}, {200, 300}});

    spawn_color_picker_window(&moga, {{550, 100}, {200, 200}});

    spawn_tool_picker_window(&moga, {{200, 100}, {200, 200}});

    // ==================================================================================

    opt_panel->add_spaceholder(5);

    // ==================================================================================

    v_TextField *txt = new v_TextField({50, 100});
    moga.add_view(txt);

    txt->e_text_changed.add(new DebugTextChange);
    txt->set_string("text_input");
    // txt->set_number(123123);

    auto dw = new v_DialogWindow("How are you?", 250, 10, -30);
    moga.add_view(dw);
    // AbstractView *v = nullptr;
    dw->add_field("Width", 50, "px");
    dw->add_field("Height", 50, "px");
    dw->add_field("Free Text", 100);

    dw->add_text_button("Accept");
    dw->add_accept_button("Accept");
    dw->add_decline_button("Accept");

}
