#pragma once


#include "labled_view.h"
#include "../renderables/renderables.h"


extern const double HIGHLIGHTER_ON_COEF;


class v_Highlighter;

class HighlighterPressAcceptor : public EventAcceptor<v_Highlighter, Event::MousePress> {
public:
    HighlighterPressAcceptor(v_Highlighter *highlighter);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};


class HighlighterMoveAcceptor : public EventAcceptor<v_Highlighter, Event::MouseMove> {
public:
    HighlighterMoveAcceptor(v_Highlighter *highlighter);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class HighlighterDeactivateVisualy : public EventAcceptor<v_Highlighter, Event::ActivityToggle> {
public:
    HighlighterDeactivateVisualy(v_Highlighter *highlighter);

    EventAccResult operator()(const Event::ActivityToggle &event, const EventAccResult *cur_res = nullptr) override;
};


class v_Highlighter : public AbstractLabledView {
    bool cursor_inside;
    double highlight_coef;

    HighlighterMoveAcceptor on_move;

    friend HighlighterPressAcceptor;
    friend HighlighterMoveAcceptor;
    friend HighlighterDeactivateVisualy;

public:
    SmartColor *color;

    v_Highlighter(const ViewBody &body, SmartColor *color, AbstractView *parent = nullptr, double highlight_coef = HIGHLIGHTER_ON_COEF);
    virtual ~v_Highlighter();

    virtual void render(Renderer *renderer) override;
    // virtual void subrender(Renderer *renderer) override;

    void add_label(const char *text, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr);
};
