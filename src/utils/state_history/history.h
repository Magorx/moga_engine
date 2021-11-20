#pragma once


#include <deque>

#include "state.h"


class History {
    std::deque<HistoryState*> prev_states;
    std::deque<HistoryState*> next_states;

    size_t max_states_cnt;

    HistoryState *cur_state;

public:
    History(size_t max_states_cnt);

    void undo();
    void redo();

    void add(HistoryState *state);

    void update_cur();
    HistoryState *cur() { return cur_state; }
};
