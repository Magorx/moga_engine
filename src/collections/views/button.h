#pragma once


#include "view/view.h"
#include "../renderables/renderables.h"


class v_Button : public View {
    char *lable;

public:
    v_Button(const ViewBody &body, SmartColor *color, Lambda *reaction = nullptr, char *lable = nullptr):
    View(body, new r_Rectangle(body.position, body.size, color)),
    lable(lable)
    {}

    void bind(Lambda *reaction);
};
