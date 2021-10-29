#pragma once


#include "highlighter.h"
#include "redactor/canvas.h"
#include "visual/renderer/renderer.h"

class CanvasPressAcceptor;
class CanvasReleaseAcceptor;
class CanvasMoveAcceptor;


class v_Canvas : public v_Highlighter {
    Canvas *canvas;

    friend CanvasPressAcceptor;
    friend CanvasReleaseAcceptor;
    friend CanvasMoveAcceptor;

public:
    v_Canvas(const ViewBody &body, Renderer *renderer, ToolManager *tool_manager);
    virtual ~v_Canvas();
};


class CanvasPressAcceptor : public EventAcceptor<v_Canvas, Event::MousePress> {
    friend v_Canvas;
public:
    CanvasPressAcceptor(v_Canvas *button);
    virtual ~CanvasPressAcceptor();

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class CanvasReleaseAcceptor : public EventAcceptor<v_Canvas, Event::MouseRelease> {
    friend v_Canvas;
public:
    CanvasReleaseAcceptor(v_Canvas *button);
    virtual ~CanvasReleaseAcceptor();

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class CanvasMoveAcceptor : public EventAcceptor<v_Canvas, Event::MouseMove> {
    friend v_Canvas;
public:
    CanvasMoveAcceptor(v_Canvas *button);
    virtual ~CanvasMoveAcceptor();

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};
