#pragma once


#include "MogaEngine.h"

#include "tools/tool_manager.h"
#include "tools/brush.h"


class RedactorEngine : public MogaEngine {
    ToolManager *tool_manager;

public:
    RedactorEngine(const char  *window_name,
                   const size_t screen_width,
                   const size_t screen_height,
                   const size_t pixel_size);
    
    inline ToolManager *get_tool_manager() { return tool_manager; }

    inline Vec2d random_screen_pos() { return {vec3d_randdouble(0, screen_width), vec3d_randdouble(0, screen_height)}; }
};
