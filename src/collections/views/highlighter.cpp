#include "highlighter.h"


const double HIGHLIGHTER_ON_COEF = 1.2;


v_Highlighter::v_Highlighter(const ViewBody &body, SmartColor *color, AbstractView *parent):
AbstractLabledView(body, nullptr, parent),
cursor_inside(false),
on_move(this),
color(color)
{
    e_mouse_press.add(new HighlighterPressAcceptor(this));
    e_mouse_move.add(new HighlighterMoveAcceptor(this));
}

void v_Highlighter::render(Renderer *renderer) {
    RGBA cur_color = cursor_inside ? (Color) (color->rgb() * HIGHLIGHTER_ON_COEF) : color->rgb();

    renderer->draw_rectangle(body.position, body.size, cur_color);

    subrender(renderer);
}

void v_Highlighter::subrender(Renderer *renderer) {
    renderer->shift(body.position);

    for (size_t i = 0; i < subviews.size(); ++i) {
        subviews[i]->render(renderer);
    }

    renderer->shift(-body.position);
}

void v_Highlighter::add_label(const char *lable, int char_size, SmartColor *font_color, SmartColor *back_color) {
    v_Text *text = new v_Text(ViewBody{body.size / 2, body.size / 2}, lable, char_size, font_color, back_color);

    text->centrized = true;
    add_subview(text);
}

void v_Highlighter::tick(const double, const double) {
    if (texture) {
        texture->set_position(body.get_position());
    }
}


HighlighterPressAcceptor::HighlighterPressAcceptor(v_Highlighter *button) : EventAcceptor(button) {}

EventAccResult HighlighterPressAcceptor::operator()(const Event::MousePress &event) {
    v_Highlighter *hl = acceptor;

    if (hl->is_inside(event.position)) {
        hl->cursor_inside = true;
        return EventAccResult::cont;
    } else {
        hl->cursor_inside = false;
        return EventAccResult::none;
    }
}

HighlighterMoveAcceptor::HighlighterMoveAcceptor(v_Highlighter *button) : EventAcceptor(button) {}

EventAccResult HighlighterMoveAcceptor::operator()(const Event::MouseMove &event) {
    v_Highlighter *hl = acceptor;

    if (hl->is_inside(event.to, event.from)) {
        hl->cursor_inside = hl->is_inside(event.to);
        return EventAccResult::cont;
    } else {
        return EventAccResult::none;
    }
}
