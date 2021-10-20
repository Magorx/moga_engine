#include "labeled_view.h"


AbstractLabledView::AbstractLabledView(const ViewBody &body, AbstractView *parent, bool to_pass_inactive, bool is_shown) :
v_Hideable(body, parent, to_pass_inactive, is_shown),
v_label(nullptr)
{}

AbstractLabledView::~AbstractLabledView() {
    delete v_label;
}

void AbstractLabledView::add_label(const char *label, int char_size, SmartColor *font_color, SmartColor *back_color) {
    v_Text *text = new v_Text(ViewBody{body.size / 2, body.size / 2}, label, char_size, font_color, back_color);

    text->centrized = true;

    if (v_label) {
        delete v_label;
        delete_subview(v_label);
    }

    v_label = text;
    // add_subview(v_label);
}

void AbstractLabledView::render(Renderer *renderer) {
    if (v_label) {
        renderer->shift( body.position);
        v_label->render(renderer);
        renderer->shift(-body.position);
    }
}

void AbstractLabledView::refit() {
    AbstractView::refit();

    if (v_label) {
        v_label->get_body().set_position({body.size / 2});
        v_label->get_body().set_size({body.size / 2});
    }
}