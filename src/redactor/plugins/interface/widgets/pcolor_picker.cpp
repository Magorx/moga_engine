#include "pcolor_picker.h"
#include "collections/styles/text.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "redactor/engine.h"


FWD_ALL_FOR_CLASS_(WidgetColorPicker)


class PColorChanged : public EventAcceptor<WidgetColorPicker, Event::ColorChanged> {
public:
    PColorChanged(WidgetColorPicker *widget) : EventAcceptor(widget) {}

    virtual EventAccResult operator()(const Event::ColorChanged &event, const EventAccResult * = nullptr) override {
        auto field = acceptor->get_widget();
        auto &handler = field->get_handler();
        if (handler) {
            handler(event.color.ui32);
        }

        return EventAccResult::none;
    }
};


WidgetColorPicker::WidgetColorPicker(const ViewBody &body, P::ColorPicker *widget) :
v_ColorPicker(
    body,
    App.app_engine->get_tool_manager()
),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetColorPicker);
    e_color_changed.add(new PColorChanged(this));
}


PluginColorPicker::PluginColorPicker(const ViewBody &body, P::Widget *parent) :
PluginWidget(body, parent),
P::ColorPicker(to_wbody(body), parent)
{
    view = new WidgetColorPicker(body, this);
    if (parent) {
        parent->add_child(this);
    }
}

P::RGBA PluginColorPicker::get_color() {
    auto picker = dynamic_cast<v_ColorPicker*>(view); if (!picker) return {0, 0, 0, 0};

    return picker->get_color().ui32;
}

void PluginColorPicker::set_color(P::RGBA color) {
    auto picker = dynamic_cast<v_ColorPicker*>(view); if (!picker) return;

    picker->set_color(color.ui32);
}
