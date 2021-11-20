#pragma once


#include "utils/state_history/history.h"


class Line;


class LineHisotryState : public HistoryState {
protected:
    Line *line;
    char *str;
    int from;
    int to;

public:
    bool prev_space = false;

    LineHisotryState(Line *line, int from, int to);

    ~LineHisotryState() {
        delete str;
    }


    virtual void undo() = 0;
    virtual void redo() = 0;
};


class LineHisotryStateInsertion : public LineHisotryState {
public:

    LineHisotryStateInsertion(Line *line, int from, int to);

    void undo() override final;
    void redo() override final;
};

class LineHisotryStateDeletion : public LineHisotryState {
public:
    LineHisotryStateDeletion(Line *line, int pos, int len);

    void undo() override final;
    void redo() override final;
};
