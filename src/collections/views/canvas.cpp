#include "canvas.h"


v_Canvas::v_Canvas(const ViewBody &body, Renderer *renderer, ToolManager *tool_manager) :
v_Highlighter(body)
{
    canvas = new Canvas(renderer, tool_manager, body.size);
    canvas->get_final_layer()->get_target()->setRepeated(true);

    set_appearence(new AppearenceTexture(canvas->get_final_layer()->get_texture(), {1, -1}));
    // set_appearence(new AppearenceTexture(Resources.texture.button.hide.idle));

    e_mouse_press.add(new AVMissPressBlocker(this));
    e_mouse_release.add(new AVMissReleaseBlocker(this));
    e_mouse_move.add(new AVMissMoveBlocker(this));

    e_mouse_press.add(new CanvasPressAcceptor(this));
    e_mouse_release.add(new CanvasReleaseAcceptor(this));
    e_mouse_move.add(new CanvasMoveAcceptor(this));
}

v_Canvas::~v_Canvas() {
    delete appearence;
}

CanvasPressAcceptor::CanvasPressAcceptor(v_Canvas *canvas) : EventAcceptor(canvas) {}
CanvasPressAcceptor::~CanvasPressAcceptor() {}

EventAccResult CanvasPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    acceptor->canvas->on_mouse_down(event.position);

    acceptor->canvas->flush_to_final();

    return EventAccResult::done;
}

CanvasReleaseAcceptor::CanvasReleaseAcceptor(v_Canvas *canvas) : EventAcceptor(canvas) {}
CanvasReleaseAcceptor::~CanvasReleaseAcceptor() {}

EventAccResult CanvasReleaseAcceptor::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    acceptor->canvas->on_mouse_up(event.position);

    acceptor->canvas->flush_to_final();

    return EventAccResult::done;
}

CanvasMoveAcceptor::CanvasMoveAcceptor(v_Canvas *canvas) : EventAcceptor(canvas) {}
CanvasMoveAcceptor::~CanvasMoveAcceptor() {}

EventAccResult CanvasMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    acceptor->canvas->on_mouse_move(event.from, event.to);

    acceptor->canvas->flush_to_final();

    return EventAccResult::done;
}
