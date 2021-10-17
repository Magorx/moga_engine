#include "menu.h"


v_Menu::v_Menu(std::vector<const char*> button_lables, SmartColor *button_color, SmartColor *text_color, SmartColor *back_color, int char_size, Vec2d button_scale):
v_VerticalLayout({{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, 0, nullptr, back_color),
button_color(button_color),
text_color(text_color),
char_size(char_size),
button_scale(button_scale)
{
    lables.resize(button_lables.size());
    for (size_t i = 0; i < lables.size(); ++i) {
        lables[i] = strdup(button_lables[i]);

        v_Highlighter *ab = new v_Highlighter({{0, 0}, {0, 0}}, button_color);
        ab->add_label(lables[i], char_size, text_color);
        layout_add(ab, button_scale.y());
    }

    refit();
}

v_Menu::~v_Menu() {
    for (auto lable : lables) {
        free(lable);
    }
}

AbstractView *v_Menu::operator[](size_t i) {
    assert(i < lables.size() && "trying to get button[i] from submenu of smaller size");
    return subviews[i];
}

void v_Menu::refit() {
    AbstractView::refit();
    recalculate_size();
    recalculate_fit_body();
    layout_refit();
}

void v_Menu::recalculate_size() {
    double sum_y_size = 0;
    double max_x_size = 0;

    for (size_t i = 0; i < lables.size(); ++i) {
        const char *lable = lables[i];

        Vec2d text_size = Renderer::get_text_size(lable, char_size);
        
        max_x_size = fmax(max_x_size, text_size.x());
        sum_y_size += text_size.y();
    }

    body.size = Vec2d(max_x_size, sum_y_size) * button_scale;

    printf("MY SIZE %g %g\n", body.size.x(), body.size.y()); 
}