#pragma once


class HistoryState {
public:
    virtual ~HistoryState() {}
    virtual void undo() = 0;
    virtual void redo() = 0;

    virtual bool empty() { return false; }
};
