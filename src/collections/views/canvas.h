#pragma once


#include "highlighter.h"
#include "redactor/canvas.h"
#include "visual/renderer/renderer.h"


class v_Canvas : public v_Highlighter {
    Canvas *canvas;

public:
    v_Canvas(const ViewBody &body, Renderer *renderer, ToolManager *tool_manager);
    virtual ~v_Canvas();
};
