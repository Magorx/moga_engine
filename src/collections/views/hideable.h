#pragma once


#include "view/view.h"


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

class HideableActivatorAcceptor : public EventAcceptor<v_Hideable, Event::Activator> {
public:
    HideableActivatorAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::Activator &event) override;
};


class v_Hideable : public AbstractView {
    friend HideablePressAcceptor;
    friend HideableReleaseAcceptor;
    friend HideableMoveAcceptor;
    friend HideableActivatorAcceptor;

public:
    bool to_pass_inactive;
    bool _is_active;

    v_Hideable(const ViewBody &body, AbstractView *parent = nullptr, bool to_pass_inactive = false, bool is_shown = true);

    virtual void render(Renderer *renderer) override;

    inline void activate()   { _is_active  = false; }
    inline void deactivate() { _is_active  = true;  }
    inline void toggle()     { _is_active ^= true;  }

    inline bool is_active() { return _is_active; }
};
