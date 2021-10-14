#pragma once


#include "highlighter.h"


class v_Hideable;


class HideablePressAcceptor : public EventAcceptor<v_Hideable, Event::MousePress> {
public:
    HideablePressAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::MousePress &event) override;
};

class HideableReleaseAcceptor : public EventAcceptor<v_Hideable, Event::MouseRelease> {
public:
    HideableReleaseAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::MouseRelease &event) override;
};

class HideableMoveAcceptor : public EventAcceptor<v_Hideable, Event::MouseMove> {
public:
    HideableMoveAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::MouseMove &event) override;
};



class v_Hideable : public v_Highlighter {
    bool is_shown;

    friend HideablePressAcceptor;
    friend HideableReleaseAcceptor;
    friend HideableMoveAcceptor;

public:
    v_Hideable(const ViewBody &body, SmartColor *color = nullptr, AbstractView *parent = nullptr, bool is_shown = false, double highlight_coef = HIGHLIGHTER_ON_COEF);

    virtual void render(Renderer *renderer) override;

    inline void hide()   { is_shown = false; }
    inline void reveal() { is_shown = true;  }

    inline bool is_active() { return is_shown; }
};
