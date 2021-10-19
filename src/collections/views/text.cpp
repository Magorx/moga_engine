#include "text.h"


v_Text::v_Text(const ViewBody &body, const char *label_, int char_size, SmartColor *font_color, SmartColor *back_color, bool centrized, AbstractView *parent) :
AbstractView(body, parent),
label(nullptr),
char_size(char_size),
back_color(back_color),
font_color(font_color),
centrized(centrized)
{
    if (!label_ || !font_color) {
        printf("eror nullptr in the text colors\n");
    }

    label = strdup(label_);
}

v_Text::~v_Text() {
    free(label);
}

void v_Text::render(Renderer *renderer) {
    if (back_color) {
        renderer->draw_text(label, char_size, body.position, font_color->rgb(), back_color->rgb(), true, centrized);
    } else {
        renderer->draw_text(label, char_size, body.position, font_color->rgb(), {0, 0, 0}, false, centrized);
    }
}
