#pragma once


#include "utils/state_history/history.h"


class Line;


class LineHisotryStateInsertion : public HistoryState {
    Line *line;
    int from;
    int to;
public:
    LineHisotryStateInsertion(Line *line, int from, int to);

    void undo() override final;
    void redo() override final;
};

class LineHisotryStateDeletion : public HistoryState {
    Line *line;
    int pos;
    char *str;
public:
    LineHisotryStateDeletion(Line *line, int pos, int len);

    void undo() override final;
    void redo() override final;
};
