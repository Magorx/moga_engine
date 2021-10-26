#include "util_bar.h"
#include "button.h"


namespace StdStyle {

namespace UtilBar {

    UtilityBarStyle *basic() {
        return new  UtilityBarStyle {
            StdStyle::Button::close(),
            StdStyle::Button::hide(),
            StdStyle::Button::basic(),

            new AppearenceTexture(Resources.texture.util_bar.basic.bar)
        };
    }

}

}
