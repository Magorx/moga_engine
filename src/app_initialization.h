#pragma once

const int SCR_W  = 1200;
const int SCR_H  = 800;

#include "redactor/engine.h"
#include "scene_generation.h"
#include "responses.h"

void initialize_photoshop(RedactorEngine &moga) {
    const double scr_width = moga.get_screen_width();
    // const double scr_height = moga.get_screen_height();

    auto appr_frame_box = new AppearenceTexture(Resources.texture.frame_gray);
    v_HorizontalLayout *opt_panel = new v_HorizontalLayout({{0, 0}, {scr_width, 30}}, {{0, 0}, {1, 1}}, 0, nullptr, nullptr);
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

    spawn_color_picker_window(&moga, {{550, 100}, {200, 240}});

    spawn_tool_picker_window(&moga, {{200, 100}, {200, 200}});

    // ==================================================================================

    opt_panel->add_spaceholder(5);

    // ==================================================================================

    // open_img_button->e_clicked.emit({});
    new_canvas_button->e_clicked.emit({});

    const char *str = "aaaaaa asdas    asda as a";
    for (const char *c = str; *c; ++c) {
        moga.main_view->e_text_enter.emit({(uint32_t) *c});
    }
    // moga.main_view->e_key_down.emit({Keyboard::Key::enter});
    // moga.main_view->e_key_up.emit({Keyboard::Key::enter});

    // ==================================================================================

}
