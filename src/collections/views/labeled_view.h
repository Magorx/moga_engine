#pragma once


#include "hideable.h"
#include "text.h"


class AbstractLabledView : public v_Hideable {
protected:
    v_Text *v_label;
public:
    AbstractLabledView(const ViewBody &body, AbstractView *parent = nullptr, bool to_block_covered = false, bool to_pass_inactive = false, bool is_shown = true);

    virtual ~AbstractLabledView();

    virtual void render(Renderer *renderer) override;

    void add_label(const char *label, int char_size, SmartColor *font_color, SmartColor *back_color = nullptr);

    virtual void refit() override;
};
