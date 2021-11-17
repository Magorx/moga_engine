#pragma once


template <typename OBJ_T>
class HistoryState {
protected:
    OBJ_T *target;

public:
    HistoryState(OBJ_T *target) : target(target) {}

    virtual void undo() = 0;
    virtual void redo() = 0;
};
