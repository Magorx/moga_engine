#include "window.h"


// v_Window::v_Window(const char *name, const ViewBody &body, SmartColor *header_color, bool draggable, double header_size, AbstractView *parent, double highlight_coef) :
// v_Highlighter(body, nullptr, parent, highlight_coef),
// header(new v_UtilityTab({body.size.x(), header_size}, header_color)),
// content(nullptr),
// text_color({230, 230, 210})
// {
//     header->get_body().position.content[1] -= header_size;
//     add_subview(header);
    
//     if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this, false));

//     header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    
//     header->add_label(name, 15, &text_color);
    
//     set_focuseable(true);
// }

v_Window::v_Window(const char *name, const ViewBody &body, WindowStyle *style, bool draggable, double header_size, AbstractView *parent) :
v_Highlighter({body.position, {body.size.x(), body.size.y() + header_size}}, nullptr, parent, 0, true),
header(new v_UtilityTab({body.size.x() + PX_WINDOW_PADDING * 2, header_size}, style ? style->header : nullptr)),
content(new v_Highlighter({{0, 0}, body.size}, nullptr, nullptr, 0)),
text_color(RColor{255, 255, 255})
{
    if (style->body) {
        auto frame = new v_Highlighter({-padding, body.size + Vec2d{padding * 2, 2 * padding}}, nullptr, nullptr, 0);
        frame->set_appearence(style->body);
        add_subview(frame);
    }

    header->get_body().position.content[1] -= header_size + PX_UTIL_BUTTON_SIZE;
    header->get_body().position.content[0] -= padding;
    add_subview(header);
    
    if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this, false));

    header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    header->get_button_hide()->e_toggle_activity.add(new HideableActivityToggleAcceptor(this));
    
    header->add_label(name, Resources.font.size.basic_header, text_color.rgb());
    
    set_focuseable(true);

    auto content_appr = new AppearenceColor({255, 255, 255, 255});
    Resources.add_appr(content_appr);
    content->set_appearence(content_appr);
    add_subview(content);

    header->focus();
    content->focus();
}

void v_Window::add_subview(AbstractView *subview) {
    if (!subview) return;

    subview->get_body().position.content[1] += header->get_body().size.content[1];
    subview->get_body().position.content[0] += padding;
    v_Highlighter::add_subview(subview);
}

void v_Window::set_content_color(RColor color) {
    auto content_appr = new AppearenceColor(color);
    Resources.add_appr(content_appr);
    content->set_appearence(content_appr);
}
