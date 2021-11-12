#include "cursor.h"


Cursor::Cursor(LinePos pos) :
pos(pos),
anchor(pos)
{}

void Cursor::fix_anchor()  { anchor_fixed = true; }
void Cursor::free_anchor() { anchor_fixed = false; }

void Cursor::pull_anchor() {
    if (!anchor_fixed) {
        anchor = pos;
    }
}

void Cursor::set(int idx) {
    pos.set(idx);
    pull_anchor();
}

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
    pull_anchor();
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
    pull_anchor();
}

void Cursor::move_home() {
    pos.move_home();
    pull_anchor();
}

void Cursor::move_end() {
    pos.move_end();
    pull_anchor();
}


void Cursor::select_all(bool to_fix_anchor) {
    anchor.move_home();
    pos.move_home();
    fix_anchor();
    pos.move_end();
    if (!to_fix_anchor) {
        free_anchor();
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
