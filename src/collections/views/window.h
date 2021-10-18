#pragma once


#include "view/view.h"
#include "utility_tab.h"


class v_Window : public v_Highlighter {
    v_UtilityTab *header;
public:
    v_Window(const ViewBody &body, double header_size, SmartColor *header_color = new SmartColor({30, 30, 30}), AbstractView *parent = nullptr, double highlight_coef = 1);

    virtual void add_subview(AbstractView *subview) override;
};
