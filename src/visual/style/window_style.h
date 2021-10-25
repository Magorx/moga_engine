#pragma once


#include "style.h"
#include "mouse_reaction_style.h"
#include "utility_bar_style.h"


struct WindowStyle : public Style {
    UtilityBarStyle *header;
    Appearence *body;
};
