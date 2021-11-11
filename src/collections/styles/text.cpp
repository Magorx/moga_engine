#include "text.h"


namespace StdStyle {

namespace Text {

    TextStyle *basic() {
        return new TextStyle {
            20,
            Resources.font.basic,
            Resources.font.color.basic_header,
            {0, 0, 0, 0}
        };
    }

    TextStyle *header() {
        return new TextStyle {
            15,
            Resources.font.basic,
            Resources.font.color.basic_header,
            {0, 0, 0, 0}
        };
    }

    TextStyle *menu() {
        return new TextStyle {
            15,
            Resources.font.basic,
            Resources.font.color.basic_menu,
            {0, 0, 0, 0}
        };
    }

}

}
