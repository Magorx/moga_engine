#include "window.h"
#include "util_bar.h"

namespace StdStyle {

namespace Window {

    WindowStyle *basic() {
        return new WindowStyle {
            StdStyle::UtilBar::basic(),
            
            new AppearenceTexture(Resources.color.alpha_blue)
        };
    }

}

}
