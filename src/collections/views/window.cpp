#include "window.h"


v_Window::v_Window(const char *name, const ViewBody &body, double header_size, SmartColor *header_color, bool draggable, AbstractView *parent, double highlight_coef) :
v_Highlighter(body, nullptr, parent, highlight_coef),
header(new v_UtilityTab({body.size.x(), header_size}, header_color)),
text_color({255, 10, 10})
{
    header->get_body().position.content[1] -= header_size;
    add_subview(header);
    
    if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this));

    header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    
    header->add_label(name, 15, &text_color);
    
    set_focuseable(true);
}

v_Window::v_Window(const char *name, const ViewBody &body, double header_size, res_WindowResources &res, bool draggable, AbstractView *parent) :
v_Highlighter({body.position, {body.size.x(), body.size.y() + header_size}}, nullptr, parent, 0),
header(new v_UtilityTab({body.size.x(), header_size}, res.util_bar)),
text_color({255, 10, 10})
{
    header->get_body().position.content[1] -= header_size;
    add_subview(header);
    
    if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this));

    header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    
    header->add_label(name, 15, &text_color);
    
    set_focuseable(true);
}

void v_Window::add_subview(AbstractView *subview) {
    if (!subview) return;

    subview->get_body().position.content[1] += header->get_body().size.content[1];
    v_Highlighter::add_subview(subview);
}
