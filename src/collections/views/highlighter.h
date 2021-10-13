#pragma once


#include "view/view.h"
#include "text.h"
#include "../renderables/renderables.h"


extern const double HIGHLIGHTER_ON_COEF;


class v_Highlighter;


class HighlighterMoveAcceptor : public EventAcceptor<v_Highlighter, Event::MouseMove> {
public:
    HighlighterMoveAcceptor(v_Highlighter *button);

    EventAccResult operator()(const Event::MouseMove &event) override;
};


class v_Highlighter : public AbstractView {
    bool cursor_inside;

    SmartColor *color;

    HighlighterMoveAcceptor on_move;

    friend HighlighterMoveAcceptor;

public:
    v_Highlighter(const ViewBody &body, SmartColor *color, AbstractView *parent = nullptr);

    virtual void tick(const double = 0, const double = 0) override;
    virtual void render(Renderer *renderer) override;
    virtual void subrender(Renderer *renderer) override;

    void add_label(const char *text, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr);

    void press(Vec2d click);
    void move(Vec2d from, Vec2d to);
    void released(Vec2d click);

    void press();
    void unpress();
};
