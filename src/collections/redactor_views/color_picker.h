#pragma once


#include "utils.h"


extern const double PX_COLOR_PICKER_PADDING;


class v_ColorPicker : public v_Highlighter {
    ToolManager *tool_manager;

    v_VerticalLayout *v_vert_layout;
    // v_HorizontalLayout *v_horz_layout;

    v_Highlighter *v_field;
    v_Highlighter *v_spectrum;

    Appearence *appr_field;
    Appearence *appr_spectrum;

    v_Magnetic *v_dot_field;
    v_Magnetic *v_dot_spectrum;

    Appearence *appr_dot_field;
    Appearence *appr_dot_spectrum;

public:
    v_ColorPicker(const ViewBody &body, ToolManager *tool_manager);

    virtual ~v_ColorPicker();
};
