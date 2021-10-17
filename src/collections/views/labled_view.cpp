#include "labled_view.h"


AbstractLabledView::AbstractLabledView(const ViewBody &body, AbstractView *parent, bool to_pass_inactive, bool is_shown) :
v_Hideable(body, parent, to_pass_inactive, is_shown),
v_lable(nullptr)
{}

AbstractLabledView::~AbstractLabledView() {
    if (v_lable) {
        delete v_lable;
    }
}

void AbstractLabledView::add_label(const char *lable, int char_size, SmartColor *font_color, SmartColor *back_color) {
    v_Text *text = new v_Text(ViewBody{body.size / 2, body.size / 2}, lable, char_size, font_color, back_color);

    text->centrized = true;

    if (v_lable) {
        delete v_lable;
    }
    v_lable = text;

    add_subview(v_lable);
}

void AbstractLabledView::refit() {
    AbstractView::refit();

    if (v_lable) {
        v_lable->get_body().set_position({body.size / 2});
        v_lable->get_body().set_size({body.size / 2});
    }
}