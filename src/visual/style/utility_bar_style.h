#pragma once


#include "style.h"
#include "mouse_reaction_style.h"


struct UtilityBarStyle : public Style {
    MouseReactionStyle *close_button;
    MouseReactionStyle *hide_button;

    MouseReactionStyle *accessory;

    Appearence *bar;

    UtilityBarStyle(MouseReactionStyle *close_button, MouseReactionStyle *hide_button, MouseReactionStyle *accessory, Appearence *bar) :
    close_button(close_button),
    hide_button(hide_button),
    accessory(accessory),
    bar(bar)
    {}

    virtual ~UtilityBarStyle() {
        delete close_button;
        delete hide_button;
        delete accessory;
        delete bar;
    }
};
