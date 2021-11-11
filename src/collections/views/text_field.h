#pragma once


#include "highlighter.h"
#include "collections/styles/text.h"
#include "utils/text_line/line.h"


class TextEnterAcceptor;
class KeyDownTextFieldAcceptor;
class KeyUpTextFieldAcceptor;


class v_TextField : public v_Highlighter {
    Line line;
    TextStyle *style;

    v_Highlighter *v_cursor;
    v_Highlighter *v_selection;

    bool redactable;
    bool insert = false;
    
    int shifted = 0;
    int ctrled = 0;

    friend TextEnterAcceptor;
    friend KeyDownTextFieldAcceptor;
    friend KeyUpTextFieldAcceptor;

public:
    v_TextField(const ViewBody &body, TextStyle *style = StdStyle::Text::basic(), bool redactable = true);
    virtual ~v_TextField();

    virtual void render(Renderer *renderer) override;

    void add_char(char c);
    void display();

    void copy_to_clipboard();
    void paste_from_clipboard();
};


class TextEnterAcceptor : public EventAcceptor<v_TextField, Event::TextEnter> {
    friend v_TextField;

public:
    TextEnterAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::TextEnter &event, const EventAccResult *cur_res = nullptr) override;
};

class KeyDownTextFieldAcceptor : public EventAcceptor<v_TextField, Event::KeyDown> {
    friend v_TextField;

public:
    KeyDownTextFieldAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult *cur_res = nullptr) override;
};

class KeyUpTextFieldAcceptor : public EventAcceptor<v_TextField, Event::KeyUp> {
    friend v_TextField;

public:
    KeyUpTextFieldAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::KeyUp &event, const EventAccResult *cur_res = nullptr) override;
};
