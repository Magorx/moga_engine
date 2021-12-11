#pragma once


#include "redactor/plugin_std/std.hpp"


class WidgetFactory : public P::WidgetFactory {
    virtual P::Button      *button       (const P::WBody &body, P::Widget *parent = nullptr) const override;
    virtual P::Button      *button       (const P::Vec2f &pos,  const char *capt, P::Widget *parent = nullptr) const override;
    virtual P::Slider      *slider       (P::Slider::Type type, const P::WBody &body, P::Widget *parent = nullptr) const override;
    virtual P::TextField   *text_field   (const P::WBody &body, P::Widget *parent = nullptr) const override;
    virtual P::Window      *window       (const char *name, const P::WBody &body, P::Widget *parent = nullptr) const override;
    virtual P::ColorPicker *color_picker (const P::WBody &body, P::Widget *parent = nullptr) const override;
    virtual P::Label       *label        (const P::WBody &body, P::Widget *parent = nullptr) const override;
    virtual P::Widget      *abstract     (const P::WBody &body, P::Widget *parent = nullptr) const override;
};
