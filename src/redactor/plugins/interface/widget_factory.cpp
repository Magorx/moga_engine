#include "widget_factory.h"

#include "widgets/pwidget.h"
#include "widgets/pwindow.h"
#include "widgets/pbutton.h"
#include "widgets/pslider.h"

P::Button *WidgetFactory::button(const P::WBody &body, P::Widget *parent) const {
    return new PluginButton(to_vbody(body), parent);
}

P::Button *WidgetFactory::button(const P::Vec2f &pos, const char *caption, P::Widget *parent) const {
    return new PluginButton({pos.x, pos.y}, caption, parent);
}

P::Slider *WidgetFactory::slider(P::Slider::Type type, const P::WBody &body, P::Widget *parent) const {
    return new PluginSlider(type, to_vbody(body), parent);
}

P::TextField *WidgetFactory::text_field(const P::WBody &body, P::Widget *parent) const {

}

P::Window *WidgetFactory::window(const char *name, const P::WBody &body, P::Widget *parent) const {
    return new PluginWindow(name, to_vbody(body), parent);
}

P::ColorPicker *WidgetFactory::color_picker(const P::WBody &body, P::Widget *parent) const {

}

P::Label *WidgetFactory::label(const P::WBody &body, P::Widget *parent) const {

}

P::Widget *WidgetFactory::abstract(const P::WBody &body, P::Widget *parent) const {
    return new PluginWidget(to_vbody(body), parent, true);
}
