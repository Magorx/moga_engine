#include "line_history.h"
#include "line.h"


LineHisotryState::LineHisotryState(Line *line, int from, int to) : 
line(line),
str(nullptr),
from(from),
to(to)
{
    if (line->c_str()) {
        str = (char*) calloc(to - from + 2, sizeof(char));
        memcpy(str, line->c_str() + from, to - from);
    }
}


LineHisotryStateInsertion::LineHisotryStateInsertion(Line *line, int from, int to) :
LineHisotryState(line, from, to)
{}

void LineHisotryStateInsertion::undo() {
    line->free_anchors();
    line->cursor_set(from);
    line->fix_anchors();
    line->cursor_set(to);
    line->erase_prev();
}

void LineHisotryStateInsertion::redo() {
    line->free_anchors();
    line->cursor_set(from);
    line->insert_str(str);
}


LineHisotryStateDeletion::LineHisotryStateDeletion(Line *line, int from, int to) :
LineHisotryState(line, from, to)
{}

void LineHisotryStateDeletion::undo() {
    line->free_anchors();
    line->cursor_set(from);
    line->insert_str(str);
}

void LineHisotryStateDeletion::redo() {
    line->free_anchors();
    line->cursor_set(from);
    line->fix_anchors();
    line->cursor_set(to);
    line->erase_prev();
}
