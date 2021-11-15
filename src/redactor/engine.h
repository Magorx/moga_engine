#pragma once


#include "MogaEngine.h"

#include "tools/tool_manager.h"
#include "tools/tools.h"


const int TOOL_BRUSH_IDX = 0;
const int TOOL_ERASER_IDX = 1;
const int TOOL_PIPETTE_IDX = 2;


class RedactorEngine : public MogaEngine {
    ToolManager *tool_manager;

public:
    RedactorEngine(const char  *window_name,
                   const size_t screen_width,
                   const size_t screen_height,
                   const size_t pixel_size);
    
    inline ToolManager *get_tool_manager() { return tool_manager; }
};
