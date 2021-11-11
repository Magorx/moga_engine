#include "cursor.h"


Cursor::Cursor(LinePos pos) :
pos(pos),
anchor(pos)
{}

void Cursor::fix_anchor()  { anchor_fixed = true; }
void Cursor::free_anchor() { anchor_fixed = false; }

void Cursor::move_r() {
    if (is_selection() && !anchor_fixed) {
        if (anchor() > pos()) {
            pos = anchor;
        } else {
            anchor = pos;
        }
        return;
    }

    pos.move_r();
    if (!anchor_fixed) {
        anchor = pos;
    }
}

void Cursor::move_l() {
    if (is_selection() && !anchor_fixed) {
        if (anchor() < pos()) {
            pos = anchor;
        } else {
            anchor = pos;
        }
        return;
    }

    pos.move_l();
    if (!anchor_fixed) {
        anchor = pos;
    }
}

bool Cursor::anchor_magnet() {
    if (pos() > anchor()) {
        pos = anchor;
        return false;
    } else {
        anchor = pos;
        return true;
    }
}

bool Cursor::is_selection() {
    return pos() != anchor();
}
