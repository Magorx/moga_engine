#include "window.h"


v_Window::v_Window(const ViewBody &body, double header_size, SmartColor *header_color, AbstractView *parent, double highlight_coef) :
v_Highlighter(body, nullptr, parent, highlight_coef),
header(new v_UtilityTab({body.size.x(), header_size}, header_color))
{
    header->get_body().position.content[1] -= header_size;
    add_subview(header);
    header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this));
}

void v_Window::add_subview(AbstractView *subview) {
    if (!subview) return;

    subview->get_body().position.content[1] += header->get_body().size.content[1];
    v_Highlighter::add_subview(subview);
}