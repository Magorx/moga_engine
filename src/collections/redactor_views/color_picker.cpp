#include "color_picker.h"


const double PX_COLOR_PICKER_PADDING = 0;


v_ColorPicker::v_ColorPicker(const ViewBody &body, ToolManager *tool_manager):
v_Highlighter(body),
v_vert_layout(new v_VerticalLayout(
    {PX_COLOR_PICKER_PADDING, body.size - 2 * PX_COLOR_PICKER_PADDING},
    {0, 1}, 
    PX_UTIL_PADDING)
),
v_horz_layout(new v_HorizontalLayout(
    {0, 0},
    {0, 1},
    PX_UTIL_PADDING)
),

v_field(new v_Highlighter({0, 0})),
v_spectrum(new v_Highlighter({0, 0})),

appr_field(new AppearenceTexture(nullptr)),
appr_spectrum(new AppearenceTexture(Resources.color.spectrum)),

v_dot_field(new v_Magnetic({0, {7, 7}}, v_field->get_body())),
v_dot_spectrum(new v_Magnetic({0, {7, 7}}, v_spectrum->get_body())),

appr_dot_field(new AppearenceTexture(Resources.texture.dot)),
appr_dot_spectrum(new AppearenceTexture(Resources.texture.dot)),

field_size(v_field->get_body().size),
field(nullptr)

{
    add_subview(v_vert_layout);

    v_vert_layout->layout_add(v_field, 5);
    v_vert_layout->layout_add(v_horz_layout, 1);
    // v_horz_layout->set_appearence(new AppearenceTexture(Resources.create_color({20, 20, 20})));
    
    v_horz_layout->layout_add(v_spectrum, 1);

    v_field->set_appearence(appr_field);
    v_spectrum->set_appearence(appr_spectrum);
    appr_spectrum->set_transform({1, 0.1});
    appr_spectrum->update_image();

    v_dot_field->set_appearence(appr_dot_field);
    v_dot_field->update_bounds(v_field->get_body());

    v_dot_spectrum->set_appearence(appr_dot_spectrum);
    v_dot_spectrum->update_bounds({0, {v_spectrum->get_body().size.x(), 0}});
    v_dot_spectrum->shift_with_bounds({0, v_spectrum->get_body().size.y() / 2});

    v_field->e_mouse_press.add(new AVMissPressBlocker(v_field));
    v_spectrum->e_mouse_press.add(new AVMissPressBlocker(v_spectrum));

    v_field->add_subview(v_dot_field);
    v_spectrum->add_subview(v_dot_spectrum);

    field = new RColor[field_size.x() * field_size.y()];

    // update_field();

    v_dot_spectrum->e_fraction_changed.add(new ColorPickerSpectrumChangeAcceptor(this));
}

void v_ColorPicker::update_field() {
    Vec2d px_coord = v_dot_spectrum->get_fraction() * v_spectrum->get_body().size;
    RColor base_color = v_spectrum->get_appearence()->get_px_color(px_coord);

    
}

v_ColorPicker::~v_ColorPicker() {
    delete appr_field;
    delete appr_spectrum;

    delete[] field;
};


ColorPickerSpectrumChangeAcceptor::ColorPickerSpectrumChangeAcceptor(v_ColorPicker *color_picker) : 
EventAcceptor(color_picker)
{}

EventAccResult ColorPickerSpectrumChangeAcceptor::operator()(const Event::FractionChanged &event, const EventAccResult *) {
    acceptor->update_field();

    return EventAccResult::cont;
}
