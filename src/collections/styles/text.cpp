#include "text.h"


namespace StdStyle {

namespace Text {

    TextStyle *basic() {
        return new TextStyle {
            Resources.font.size.basic,
            Resources.font.basic,
            Resources.font.color.basic_header,
            {0, 0, 0, 0}
        };
    }

    TextStyle *header() {
        return new TextStyle {
            Resources.font.size.basic_header,
            Resources.font.basic,
            Resources.font.color.basic_header,
            {0, 0, 0, 0}
        };
    }

    TextStyle *menu() {
        return new TextStyle {
            Resources.font.size.basic_menu,
            Resources.font.basic,
            Resources.font.color.basic_menu,
            {0, 0, 0, 0}
        };
    }

}

}
