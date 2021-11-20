#include "history.h"


History::History(size_t max_states_cnt) :
max_states_cnt(max_states_cnt)
{}

void History::update_cur() {
    if (!prev_states.size()) {
        cur_state = nullptr;
    } else {
        cur_state = prev_states.back();
    }
}

void History::undo() {
    if (!prev_states.size()) return;

    HistoryState *state = prev_states.back();
    prev_states.pop_back();
    next_states.push_front(state);

    state->undo();

    update_cur();
}

void History::redo() {
    if (!next_states.size()) return;

    HistoryState *state = next_states.front();
    next_states.pop_front();
    prev_states.push_back(state);

    state->redo();

    update_cur();
}

void History::add(HistoryState *state) {
    next_states.clear();
    prev_states.push_back(state);

    while (prev_states.size() > max_states_cnt) {
        delete prev_states.front();
        prev_states.pop_front();
    }

    update_cur();
}
