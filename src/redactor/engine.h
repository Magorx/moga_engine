#pragma once


#include "MogaEngine.h"

#include "tools/tool_manager.h"
#include "tools/tools.h"

// #include ""


class RedactorEngine : public MogaEngine {
    ToolManager *tool_manager;

public:
    RedactorEngine(RWindow *window,
                   const char *name);
    
    ~RedactorEngine();
    
    inline ToolManager *get_tool_manager() { return tool_manager; }
};
