#pragma once


#include "view/view.h"
#include "utility_tab.h"


class v_Window : public v_Highlighter {
    v_UtilityTab *header;

    SmartColor text_color;
public:
    v_Window(const char *name, const ViewBody &body, double header_size, SmartColor *header_color = new SmartColor({30, 30, 30}), bool draggable = true, AbstractView *parent = nullptr, double highlight_coef = 0);

    virtual void add_subview(AbstractView *subview) override;
};
