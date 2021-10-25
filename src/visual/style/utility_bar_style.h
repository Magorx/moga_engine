#pragma once


#include "style.h"
#include "mouse_reaction_style.h"


struct UtilityBarStyle : public Style {
    MouseReactionStyle *close_button;
    MouseReactionStyle *hide_button;

    MouseReactionStyle *accessory;

    Appearence *bar;

    virtual ~UtilityBarStyle() {
        delete close_button;
        delete hide_button;
        delete accessory;
        delete bar;
    }
};
