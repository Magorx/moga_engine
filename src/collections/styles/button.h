#pragma once


#include "utils.h"


namespace StdStyle {

namespace Button {

    MouseReactionStyle *close();

    MouseReactionStyle *hide();

    MouseReactionStyle *basic();

    MouseReactionStyle *plus();

    namespace Arrow {
        MouseReactionStyle *right();
        MouseReactionStyle *left();
    }

}

}
