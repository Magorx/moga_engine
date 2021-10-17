#include "highlighter.h"


const double HIGHLIGHTER_ON_COEF = 1.2;


v_Highlighter::v_Highlighter(const ViewBody &body, SmartColor *color, AbstractView *parent, double highlight_coef):
AbstractLabledView(body, parent),
cursor_inside(false),
highlight_coef(highlight_coef),
on_move(this),
color(color)
{
    e_mouse_press.add(new HighlighterPressAcceptor(this));
    e_mouse_move.add(new HighlighterMoveAcceptor(this));
    e_toggle_activity.add(new HighlighterDeactivateVisualy(this));
}

v_Highlighter::~v_Highlighter() {}

void v_Highlighter::render(Renderer *renderer) {
    if (color) {
        RGBA cur_color = cursor_inside ? (Color) (color->rgb() * highlight_coef) : color->rgb();

        renderer->draw_rectangle(body.position, body.size, cur_color);
    }

    subrender(renderer);
}

HighlighterPressAcceptor::HighlighterPressAcceptor(v_Highlighter *highlighter) : EventAcceptor(highlighter) {}

EventAccResult HighlighterPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    v_Highlighter *hl = acceptor;

    if (hl->is_inside(event.position)) {
        hl->cursor_inside = true;
        return EventAccResult::cont;
    } else {
        hl->cursor_inside = false;
        return EventAccResult::none;
    }
}

HighlighterMoveAcceptor::HighlighterMoveAcceptor(v_Highlighter *highlighter) : EventAcceptor(highlighter) {}

EventAccResult HighlighterMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    v_Highlighter *hl = acceptor;

    if (hl->is_inside(event.to, event.from)) {
        hl->cursor_inside = hl->is_inside(event.to);
        return EventAccResult::cont;
    } else {
        return EventAccResult::none;
    }
}

HighlighterDeactivateVisualy::HighlighterDeactivateVisualy(v_Highlighter *highlighter) : EventAcceptor(highlighter) {}

EventAccResult HighlighterDeactivateVisualy::operator()(const Event::ActivityToggle &event, const EventAccResult *) {
    v_Highlighter *hl = acceptor;

    if ((event.mode & Event::ActivityToggle::State::visualy) && (event.mode & Event::ActivityToggle::State::off)) {
        hl->cursor_inside = false;
    }

    return EventAccResult::none;
}
