#pragma once


#include "style.h"
#include "mouse_reaction_style.h"


struct UtilityBarStyle : public Style {
    MouseReactionStyle *close_button;
    MouseReactionStyle *hide_button;

    MouseReactionStyle *accessory;

    Appearence *l_corner;
    Appearence *r_corner;

    Appearence *bar;

    UtilityBarStyle(MouseReactionStyle *close_button, MouseReactionStyle *hide_button, MouseReactionStyle *accessory, Appearence *bar, Appearence *l_corner, Appearence *r_corner) :
    close_button(close_button),
    hide_button(hide_button),
    accessory(accessory),
    l_corner(l_corner),
    r_corner(r_corner),
    bar(bar)
    {}

    virtual ~UtilityBarStyle() {
        delete close_button;
        delete hide_button;
        delete accessory;
        delete bar;
    }
};
