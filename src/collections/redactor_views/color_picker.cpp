#include "color_picker.h"


const double PX_COLOR_PICKER_PADDING = 5;


v_ColorPicker::v_ColorPicker(const ViewBody &body, ToolManager *tool_manager):
    v_Highlighter(body),
    v_vert_layout(new v_VerticalLayout(
        {PX_COLOR_PICKER_PADDING, body.size - PX_COLOR_PICKER_PADDING},
        {0, 1}, 
        PX_UTIL_PADDING)
    ),
    v_field(new v_Highlighter({0, 0})),
    v_spectrum(new v_Highlighter({0, 0})),
    appr_field(new AppearenceTexture(nullptr)),
    appr_spectrum(new AppearenceTexture(Resources.color.spectrum)),
    v_dot_field(new v_Magnetic({0, {5, 5}}, v_field->get_body())),
    v_dot_spectrum(new v_Magnetic({0, {5, 5}}, v_spectrum->get_body())),
    appr_dot_field(new AppearenceTexture(Resources.texture.dot)),
    appr_dot_spectrum(new AppearenceTexture(Resources.texture.dot))
    {
        add_subview(v_vert_layout);

        v_vert_layout->layout_add(v_field   , 5);
        v_vert_layout->layout_add(v_spectrum, 5);

        v_field->set_appearence(appr_field);
        v_spectrum->set_appearence(appr_spectrum);

        v_dot_field->set_appearence(appr_dot_field);
        v_dot_field->update_bounds(v_field->get_body());
        v_dot_spectrum->set_appearence(appr_dot_spectrum);
        v_dot_spectrum->update_bounds(v_spectrum->get_body());

        v_field->e_mouse_press.add(new AVMissPressBlocker(v_field));
        // v_field->e_mouse_move.add(new AVMissMoveBlocker(v_field));
        v_spectrum->e_mouse_press.add(new AVMissPressBlocker(v_spectrum));
        // v_spectrum->e_mouse_move.add(new AVMissMoveBlocker(v_spectrum));

        v_field->add_subview(v_dot_field);
        v_spectrum->add_subview(v_dot_spectrum);

        printf("size %g x %g\n", v_spectrum->get_body().size.x(), v_spectrum->get_body().size.y());
    }

v_ColorPicker::~v_ColorPicker() {
    delete appr_field;
    delete appr_spectrum;

};
