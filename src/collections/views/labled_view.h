#pragma once


#include "hideable.h"
#include "text.h"


class AbstractLabledView : public v_Hideable {
protected:
    v_Text *v_lable;
public:
    AbstractLabledView(const ViewBody &body, AbstractView *parent = nullptr, bool to_pass_inactive = false, bool is_shown = true);

    virtual ~AbstractLabledView();

    void add_label(const char *lable, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr);

    virtual void refit() override;
};
