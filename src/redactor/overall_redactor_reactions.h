#pragma once


#include "event_system/event_system.h"

#include "redactor/tools/tool_manager.h"


class ToolManagerHotkeys : public EventAcceptor<ToolManager, Event::KeyDown> {
    int prev_index = 0;

public:
    ToolManagerHotkeys(ToolManager *tool_manager) :
    EventAcceptor(tool_manager)
    {}

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult*) override {
        int cur = acceptor->get_active_tool_idx();

        acceptor->set_active_tool(event.code);

        return EventAccResult::cont;
    }
};
