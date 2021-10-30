class SpawnBallLambda : public EventReaction<Event::MousePress> {
    MogaEngine *engine;

public:
    SpawnBallLambda(MogaEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MousePress &, const EventAccResult*) override {
        gen_ball(engine);
        return EventAccResult::none;
    }
};


void spawn_canvas_window(RedactorEngine *engine, double size_x, double size_y) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("Aboba", {{200, 200}, {size_x, size_y}}, window_style);

    engine->add_view(window);

    auto canvas = new v_Canvas({0, {size_x, size_y}}, engine->visual->get_renderer(), engine->get_tool_manager());

    window->get_content()->add_subview(canvas);
}


class AddNewCanvasReaction : public EventReaction<Event::MouseRelease> {
    RedactorEngine *engine;

public:
    AddNewCanvasReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MouseRelease &, const EventAccResult*) override {

        spawn_canvas_window(engine, 300, 200);

        return EventAccResult::none;
    }
};
