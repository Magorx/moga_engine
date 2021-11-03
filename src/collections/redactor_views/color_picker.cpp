#include "color_picker.h"


const double PX_COLOR_PICKER_PADDING = 0;
const double PX_COLOR_PICKER_CIRCLE_SIZE = 10;


v_ColorPicker::v_ColorPicker(const ViewBody &body, ToolManager *tool_manager):
v_Highlighter(body),

tool_manager(tool_manager),

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

v_dot_field(new v_Magnetic({0, PX_COLOR_PICKER_CIRCLE_SIZE}, v_field->get_body())),
v_dot_spectrum(new v_Magnetic({0, PX_COLOR_PICKER_CIRCLE_SIZE}, v_spectrum->get_body())),

appr_dot_field(new AppearenceTexture(Resources.texture.dot)),
appr_dot_spectrum(new AppearenceTexture(Resources.texture.dot)),

field_size(v_field->get_body().size),
field(nullptr)

{
    add_subview(v_vert_layout);

    v_vert_layout->layout_add(v_field, 5);
    v_vert_layout->layout_add(v_horz_layout, 5);
    // v_horz_layout->set_appearence(new AppearenceTexture(Resources.create_color({20, 20, 20})));
    
    v_horz_layout->layout_add(v_spectrum, 1);

    v_field->set_appearence(appr_field);
    v_spectrum->set_appearence(appr_spectrum);
    appr_spectrum->set_transform({1, 1});
    
    appr_spectrum->update_image();

    v_dot_field->set_appearence(appr_dot_field);
    v_dot_field->update_bounds(v_field->get_body());
    
    appr_dot_field->set_screen_shift(-PX_COLOR_PICKER_CIRCLE_SIZE / 2);
    appr_dot_spectrum->set_screen_shift(-PX_COLOR_PICKER_CIRCLE_SIZE / 2);

    v_dot_spectrum->set_appearence(appr_dot_spectrum);
    v_dot_spectrum->update_bounds({0, {v_spectrum->get_body().size.x(), v_spectrum->get_body().size.y()}});
    // v_dot_spectrum->shift_with_bounds({0, v_spectrum->get_body().size.y() / 2});

    v_field->e_mouse_press.add(new AVMissPressBlocker(v_field));
    v_spectrum->e_mouse_press.add(new AVMissPressBlocker(v_spectrum));

    v_field->add_subview(v_dot_field);
    v_spectrum->add_subview(v_dot_spectrum);

    field = new RColor[field_size.x() * field_size.y()];

    field_texture = new RTexture;
    field_texture->create(field_size.x(), field_size.y());
    appr_field->get_render_mode()->texture = field_texture;

    v_dot_spectrum->e_fraction_changed.add(new ColorPickerSpectrumChangeAcceptor(this));
    v_dot_field->e_fraction_changed.add(new ColorPickerFieldChangeAcceptor(this));

    update_field();
}

void v_ColorPicker::update_field() {
    Vec2d px_coord = v_dot_spectrum->get_fraction() * v_spectrum->get_body().size;
    RColor base_color = v_spectrum->get_appearence()->get_px_color(px_coord);

    for (double y = 0; y < field_size.y(); y += 1) {
        for (double x = 0; x < field_size.x(); x += 1) {
            Vec2d frac = {x / (field_size.x() - 1), 1 - y / (field_size.y() - 1)};

            Color color = Color(base_color.r, base_color.g, base_color.b, 255);
            color *= frac.x();
            color += Color{255, 255, 255, 255} * (1.0 - frac.x());

            color *= frac.y();

            RColor final_color = color;
            final_color.a = base_color.a;
            field[(int)(y * field_size.x() + x)] = final_color;
        }
    }

    field_texture->update((sf::Uint8*) field);
    appr_field->update_image();

    update_tool_manager_color();
}

void v_ColorPicker::update_tool_manager_color() {
    Vec2d px_coord = v_dot_field->get_fraction() * v_field->get_body().size;
    RColor color = v_field->get_appearence()->get_px_color(px_coord);

    tool_manager->set_draw_color(color);
}

v_ColorPicker::~v_ColorPicker() {
    delete appr_field;
    delete appr_spectrum;
    delete field_texture;

    delete[] field;
};


ColorPickerSpectrumChangeAcceptor::ColorPickerSpectrumChangeAcceptor(v_ColorPicker *color_picker) : 
EventAcceptor(color_picker)
{}

EventAccResult ColorPickerSpectrumChangeAcceptor::operator()(const Event::FractionChanged &, const EventAccResult *) {
    acceptor->update_field();

    return EventAccResult::cont;
}

ColorPickerFieldChangeAcceptor::ColorPickerFieldChangeAcceptor(v_ColorPicker *color_picker) : 
EventAcceptor(color_picker)
{}

EventAccResult ColorPickerFieldChangeAcceptor::operator()(const Event::FractionChanged &, const EventAccResult *) {
    acceptor->update_tool_manager_color();

    return EventAccResult::cont;
}