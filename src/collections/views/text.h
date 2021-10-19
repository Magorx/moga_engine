#pragma once


#include <cstring>

#include "view/view.h"
#include "../renderables/renderables.h"


class v_Text : public AbstractView {
    char *label;
    int char_size;
    SmartColor *back_color;
    SmartColor *font_color;

public:
    bool centrized;

    v_Text(const ViewBody &body, const char *label, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr, bool centrized = false, AbstractView *parent = nullptr);
    virtual ~v_Text();

    virtual void render(Renderer *renderer) override;
};
