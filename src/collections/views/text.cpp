#include "text.h"


v_Text::v_Text(const ViewBody &body, const char *lable_, int char_size, SmartColor *font_color, SmartColor *back_color, bool centrized, AbstractView *parent) :
AbstractView(body, parent),
lable(nullptr),
char_size(char_size),
back_color(back_color),
font_color(font_color),
centrized(centrized)
{
    if (!lable_ || !font_color) {
        printf("eror nullptr in the text colors\n");
    }

    lable = strdup(lable_);
}

v_Text::~v_Text() {
    free(lable);
}

void v_Text::render(Renderer *renderer) {
    if (back_color) {
        renderer->draw_text(lable, char_size, body.position, font_color->rgb(), back_color->rgb(), true, centrized);
    } else {
        renderer->draw_text(lable, char_size, body.position, font_color->rgb(), {0, 0, 0}, false, centrized);
    }
}
