#pragma once


#include "view/view.h"
#include "text.h"

class AbstractLabledView : public AbstractView {
public:
    using AbstractView::AbstractView;

    inline void add_label(const char *lable, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr) {
        v_Text *text = new v_Text(ViewBody{body.size / 2, body.size / 2}, lable, char_size, font_color, back_color);

        text->centrized = true;
        add_subview(text);
    }
};
