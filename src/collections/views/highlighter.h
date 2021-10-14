#pragma once


#include "labled_view.h"
#include "../renderables/renderables.h"


extern const double HIGHLIGHTER_ON_COEF;


class v_Highlighter;

class HighlighterPressAcceptor : public EventAcceptor<v_Highlighter, Event::MousePress> {
public:
    HighlighterPressAcceptor(v_Highlighter *button);

    EventAccResult operator()(const Event::MousePress &event) override;
};


class HighlighterMoveAcceptor : public EventAcceptor<v_Highlighter, Event::MouseMove> {
public:
    HighlighterMoveAcceptor(v_Highlighter *button);

    EventAccResult operator()(const Event::MouseMove &event) override;
};


class v_Highlighter : public AbstractLabledView {
    bool cursor_inside;
    double highlight_coef;

    HighlighterMoveAcceptor on_move;

    friend HighlighterPressAcceptor;
    friend HighlighterMoveAcceptor;

public:
    SmartColor *color;

    v_Highlighter(const ViewBody &body, SmartColor *color, AbstractView *parent = nullptr, double highlight_coef = HIGHLIGHTER_ON_COEF);
    virtual ~v_Highlighter();

    virtual void render(Renderer *renderer) override;
    // virtual void subrender(Renderer *renderer) override;

    void add_label(const char *text, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr);
};
