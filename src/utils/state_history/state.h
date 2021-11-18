#pragma once


class HistoryState {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
};
