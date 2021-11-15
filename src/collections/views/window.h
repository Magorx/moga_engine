#pragma once


#include "view/view.h"
#include "utility_tab.h"

#include "collections/styles/window.h"


const int PX_WINDOW_PADDING = 5;


class v_Window : public v_Highlighter {
    v_UtilityTab *header;
    AbstractView *content;
    AbstractView *frame;

    double padding = PX_WINDOW_PADDING;

    SmartColor text_color;
public:
    // v_Window(const char *name, const ViewBody &body, SmartColor *header_color, bool draggable = true, double header_size = PX_UTIL_HEADER_SIZE, AbstractView *parent = nullptr, double highlight_coef = 0);
    v_Window(const char *name, const ViewBody &body, WindowStyle *style = Resources.add_style(StdStyle::Window::basic()), bool draggable = true, double header_size = PX_UTIL_HEADER_SIZE, AbstractView *parent = nullptr);

    virtual void add_subview(AbstractView *subview) override;

    virtual bool is_inside(const Vec2d &click) override { return header->is_inside(click) || content->is_inside(click); }
    virtual bool is_inside(const Vec2d &from, const Vec2d &to) override { return header->is_inside(from) || content->is_inside(from) || header->is_inside(to) || content->is_inside(to); }

    virtual AbstractView *get_first_selectable(bool from_parent = false) override;
    virtual AbstractView *get_last_selectable(bool from_parent = false) override;

    inline AbstractView *get_accessory() { return header ? header->get_accessory() : nullptr; }
    inline AbstractView *get_content() { return content; }

    inline v_UtilityTab *get_header() { return header; }

    void set_content_color(RColor color);

    void fit_frame_to_content();
};
