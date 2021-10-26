#pragma once


#include "view/view.h"
#include "utility_tab.h"

#include "visual/style/window_style.h"


const int PX_WINDOW_PADDING = 5;


class v_Window : public v_Highlighter {
    v_UtilityTab *header;

    SmartColor text_color;
public:
    v_Window(const char *name, const ViewBody &body, double header_size, SmartColor *header_color, bool draggable = true, AbstractView *parent = nullptr, double highlight_coef = 0);
    v_Window(const char *name, const ViewBody &body, double header_size, WindowStyle *style, bool draggable = true, AbstractView *parent = nullptr);

    virtual void add_subview(AbstractView *subview) override;

    inline AbstractView *get_accessory() {return header ? header->get_accessory() : nullptr;   }
};
