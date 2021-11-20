#include "line_history.h"
#include "line.h"


LineHisotryState::LineHisotryState(Line *line, int from, int to) : 
line(line),
str(nullptr),
from(from),
to(to)
{
    printf("init from %d to %d\n", from, to);
    if (line->c_str()) {
        str = (char*) calloc(to - from + 2, sizeof(char));
        memcpy(str, line->c_str() + from, to - from);
    }
}


LineHisotryStateInsertion::LineHisotryStateInsertion(Line *line, int from, int to) :
LineHisotryState(line, from, to)
{}

void LineHisotryStateInsertion::undo() {
    ScopeHistoryAdditionBlocker add_blocker(*history);

    line->free_anchors();
    line->cursor_set(from);
    line->fix_anchors();
    line->cursor_set(to);
    line->erase_prev();
    line->free_anchors();
}

void LineHisotryStateInsertion::redo() {
    ScopeHistoryAdditionBlocker add_blocker(*history);

    line->free_anchors();
    line->cursor_set(from);
    line->insert_str(str);
}


LineHisotryStateDeletion::LineHisotryStateDeletion(Line *line, int from, int to, bool right) :
LineHisotryState(line, from, to),
right(right)
{}

void LineHisotryStateDeletion::undo() {
    ScopeHistoryAdditionBlocker add_blocker(*history);

    line->free_anchors();
    line->cursor_set(from);
    line->insert_str(str);

    if (!right) {
        line->cursor_set(from);
    }
}

void LineHisotryStateDeletion::redo() {
    ScopeHistoryAdditionBlocker add_blocker(*history);

    line->free_anchors();
    line->cursor_set(from);
    line->fix_anchors();
    line->cursor_set(to);
    line->erase_prev();
}
