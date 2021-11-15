#pragma once


#include "utils.h"


extern const double PX_COLOR_PICKER_PADDING;
extern const double PX_COLOR_PICKER_CIRCLE_SIZE;


class ColorPickerSpectrumChangeAcceptor;
class ColorPickerFieldChangeAcceptor;


class v_ColorPicker : public v_Highlighter {
    friend ColorPickerSpectrumChangeAcceptor;
    friend ColorPickerFieldChangeAcceptor;

    ToolManager *tool_manager;

    v_VerticalLayout *v_vert_layout;
    v_HorizontalLayout *v_horz_layout;

    AppearenceTexture *appr_field;
    AppearenceTexture *appr_spectrum;

    v_Magnetic *v_field;
    v_Magnetic *v_spectrum;

    v_Highlighter *v_dot_field;
    v_Highlighter *v_dot_spectrum;

    AppearenceTexture *appr_dot_field;
    AppearenceTexture *appr_dot_spectrum;

    Vec2d &field_size;
    RColor *field;

    RTexture *field_texture;

    void update_field();
    void update_tool_manager_color();

public:
    v_ColorPicker(const ViewBody &body, ToolManager *tool_manager);

    virtual ~v_ColorPicker();
};

class ColorPickerSpectrumChangeAcceptor : public EventAcceptor<v_ColorPicker, Event::FractionChanged> {
    friend v_ColorPicker;
public:
    ColorPickerSpectrumChangeAcceptor(v_ColorPicker *color_picker);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};

class ColorPickerFieldChangeAcceptor : public EventAcceptor<v_ColorPicker, Event::FractionChanged> {
    friend v_ColorPicker;
public:
    ColorPickerFieldChangeAcceptor(v_ColorPicker *color_picker);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};
