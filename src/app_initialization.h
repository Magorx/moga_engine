#pragma once

const int SCR_W  = 1200;
const int SCR_H  = 800;

#include "redactor/engine.h"
#include "scene_generation.h"
#include "responses.h"

void initialize_photoshop(RedactorEngine &moga) {
    const double scr_width = moga.get_screen_width();
    // const double scr_height = moga.get_screen_height();

    auto appr_frame_box = new AppearenceTexture(App.texture.frame_gray);
    v_HorizontalLayout *opt_panel = new v_HorizontalLayout({{0, 0}, {scr_width, App.font.size.basic_menu + 4.0}}, {{0, 0}, {1, 1}}, 0, nullptr, nullptr);
    opt_panel->set_appearence(appr_frame_box);
    
    moga.add_view(opt_panel);

    // ==================================================================================

    auto new_canvas_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_canvas_button->e_clicked.add(new AddNewCanvasReaction(&moga));
    new_canvas_button->add_label("Canvas", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    auto new_picker_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_picker_button->e_clicked.add(new AddNewColorPickerReaction(&moga));
    new_picker_button->add_label("Color Picker", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(new_picker_button, 1.5);

    // ==================================================================================

    auto open_img_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    open_img_button->e_clicked.add(new OpenImage(&moga));
    open_img_button->add_label("Open Image", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(open_img_button, 1.5);

    // ==================================================================================
    auto new_tool_window = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_tool_window->add_label("Tools", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(new_tool_window, 1.5);
    new_tool_window->e_clicked.add(new AddNewToolManagerWindowReaction(&moga));
    // ==================================================================================
    auto new_effect_window = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_effect_window->add_label("Effects", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(new_effect_window, 1.5);
    new_effect_window->e_clicked.add(new AddNewEffectManagerWindowReaction(&moga));
    // ==================================================================================
    auto plugin_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    plugin_button->add_label("Plugin", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(plugin_button, 1.5);
    plugin_button->e_clicked.add(new LoadPluginDialogReaction(&moga));
    // ==================================================================================

    moga.load_plugin("resources/plugin/tool", true);
    moga.load_plugin("resources/plugin/effect", true);
    moga.load_plugin("resources/plugin/extension", true);

    // ==================================================================================

    spawn_canvas_window(&moga, {{800, 100}, {300, 400}});

    // spawn_color_picker_window(&moga, {{550, 100}, {200, 240}});

    // spawn_tool_picker_window(&moga, {{200, 100}, {200, 200}});

    // ==================================================================================

    open_img_button->e_clicked.emit({});
    moga.main_view->e_text_enter.emit({'1'});
    moga.main_view->e_text_enter.emit({'.'});
    moga.main_view->e_text_enter.emit({'j'});
    moga.main_view->e_text_enter.emit({'p'});
    moga.main_view->e_text_enter.emit({'g'});
    moga.main_view->e_key_down.emit({Keyboard::Key::enter});
    moga.main_view->e_key_up.emit({Keyboard::Key::enter});

    // new_effect_window->e_clicked.emit({});
    new_tool_window->e_clicked.emit({});

//     auto dw = new v_DialogWindow("Dialogus", 200, 5, -30);
//     dw->add_slider("Width");
//     dw->add_slider("Height");
//     dw->add_color_picker(150);

//     dw->add_accept_button("Apply");

//     moga.add_view(dw);
}
