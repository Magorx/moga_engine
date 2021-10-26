#include "window.h"


v_Window::v_Window(const char *name, const ViewBody &body, SmartColor *header_color, bool draggable, double header_size, AbstractView *parent, double highlight_coef) :
v_Highlighter(body, nullptr, parent, highlight_coef),
header(new v_UtilityTab({body.size.x(), header_size}, header_color)),
text_color({230, 230, 210})
{
    header->get_body().position.content[1] -= header_size;
    add_subview(header);
    
    if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this, false));

    header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    
    header->add_label(name, 15, &text_color);
    
    set_focuseable(true);
}

v_Window::v_Window(const char *name, const ViewBody &body, WindowStyle *style, bool draggable, double header_size, AbstractView *parent) :
v_Highlighter({body.position, {body.size.x(), body.size.y() + header_size}}, nullptr, parent, 0),
header(new v_UtilityTab({body.size.x() + PX_WINDOW_PADDING * 2, header_size}, style ? style->header : nullptr)),
text_color({255, 255, 255})
{
    header->get_body().position.content[1] -= header_size;
    add_subview(header);
    
    if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this, false));

    header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    
    header->add_label(name, 20, &text_color);
    
    set_focuseable(true);

    if (style->body) {
        auto content = new v_Highlighter({0, body.size + Vec2d{PX_WINDOW_PADDING * 2, PX_WINDOW_PADDING}}, nullptr, nullptr, 0);
        content->set_appearence(style->body);
        add_subview(content);

        content = new v_Highlighter({{PX_WINDOW_PADDING, 0}, body.size}, nullptr, nullptr, 0);
        content->set_appearence(style->body);
        add_subview(content);
    }
}

void v_Window::add_subview(AbstractView *subview) {
    if (!subview) return;

    subview->get_body().position.content[1] += header->get_body().size.content[1];
    v_Highlighter::add_subview(subview);
}
