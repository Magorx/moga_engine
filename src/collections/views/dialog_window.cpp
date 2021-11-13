#include "dialog_window.h"
#include "engine/moga_engine.h"


v_DialogWindow::v_DialogWindow(const char *name, double width, double padding, double center_offset, WindowStyle *style) :
v_Window(name, {0, {width, 0}}, style, true),
v_stretcher(v_Stretcher::Y({0, {width, 0}})),
center_offset(center_offset),
padding(padding)
{
    Resources.engine->centrize_view_body(body);
    get_header()->get_button_hide()->to_delete = true;

    get_content()->add_subview(v_stretcher);

    v_stretcher->set_zerify_normal_coord(false);
    v_stretcher->add_placehodler(padding);
    printf("stretcher %p\n", v_stretcher);
}

void v_DialogWindow::fit_content_to_fields() {
    get_content()->get_body().size.content[1] = v_stretcher->get_body().size[1];
    fit_frame_to_content();
}

v_TextField *v_DialogWindow::add_field(const char *field_name, double max_ans_length, const char *after_name) {

    v_TextField *field  = new v_TextField({0, 0}, StdStyle::Text::basic()->negative(), 0, 0, true, false);
    field->set_string(field_name, true);
    Vec2d rect_pos = {get_content()->get_body().size.x() / 2 - field->get_body().size.x() + center_offset, 0};

    v_TextField *answer = new v_TextField({{field->get_body().size.x(), 0}, max_ans_length});
    
    v_Highlighter *rect = new v_Highlighter({rect_pos, {field->get_body().size.x() + answer->get_body().size.x(), field->get_body().size.y()}});
    rect->add_subview(field);
    rect->add_subview(answer);
    
    if (after_name) {
        v_TextField *aftertext  = new v_TextField({0, 0}, StdStyle::Text::basic()->negative(), 0, 0, true, false);
        aftertext->set_string(after_name, true);
        aftertext->get_body().position.content[0] = answer->get_body().position[0] + answer->get_body().size[0];
        rect->add_subview(aftertext);
    }

    // rect->set_appearence(new AppearenceColor({100, 100, 200, 200}));
    v_stretcher->add_subview(rect);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    return answer;
}

v_Button *v_DialogWindow::add_text_button(const char *button_name, 
                                          MouseReactionStyle *button_style,
                                          TextStyle *text_style) {
    v_Button *button = new v_Button(button_name, button_style, text_style, PX_BUTTON_SIZE_PADDING);
    button->get_body().position.content[0] = get_content()->get_body().size.x() / 2 + center_offset;

    v_stretcher->add_subview(button);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    return button;
}

v_Button *v_DialogWindow::add_accept_button (const char *button_name, TextStyle *text_style) {
    return add_text_button(button_name,
                           Resources.add_style(StdStyle::Button::green()),
                           text_style);
}

v_Button *v_DialogWindow::add_decline_button(const char *button_name, TextStyle *text_style) {
    return add_text_button(button_name,
                           Resources.add_style(StdStyle::Button::red()),
                           text_style);
}

