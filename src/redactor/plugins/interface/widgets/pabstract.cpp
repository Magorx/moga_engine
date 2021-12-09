#include "pabstract.h"
#include "utils/logger.h"
#include "engine/Resources.h"


PluginWidget::PluginWidget(const ViewBody &body, PluginWidget *parent = nullptr) :
Widget({{body.position.x(), body.position.y()}, {body.size.x(), body.size.y()}}, parent),
view(new v_Highlighter(body, nullptr, true))
{
    if (parent) {
        parent->add_child(this);
    }
}

PluginWidget::PluginWidget(Widget *widget) :
Widget(widget->get_body(), widget->get_parent()),
view(nullptr)
{
    ViewBody body = {
        {widget->get_body().position.x, widget->get_body().position.y},
        {widget->get_body().size.x, widget->get_body().size.y}
    };

    view = new v_Highlighter(body);
}

PluginWidget::~PluginWidget() {
    view->to_delete = true;
}

bool PluginWidget::is_active() {
    return view->is_active();
}

bool PluginWidget::is_inside(P::Vec2f pos) {
    return view->is_inside({pos.x, pos.y});
}


bool PluginWidget::add_child(Widget *child) {
    if (!child) return false;

    PluginWidget *app_widget = dynamic_cast<PluginWidget*>(child);
    if (!app_widget) {
        app_widget = new PluginWidget(child);
    }

    view->add_subview(app_widget->get_view());

    return true;
}

bool PluginWidget::delete_child(Widget *child) {
    logger.error("PluginWidget", "delete_child is not implemented");
    return false;
}


bool PluginWidget::delete_from_parent() {
    logger.error("PluginWidget", "delete_from_parent is not implemented");
    return false;
}

void PluginWidget::hide() {
    view->set_active(false);
    on_hide(this, {});
}

void PluginWidget::show() {
    view->set_active(true);
    on_show(this, {});
}


void PluginWidget::set_caption(const char *text, size_t font_size, const P::Vec2f *pos = nullptr) {
    view->add_label(text, font_size, App.font.color.basic_header);
}
