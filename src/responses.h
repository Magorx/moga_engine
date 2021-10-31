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


class CanvasNextPrevReaction : public EventAcceptor<Canvas, Event::Clicked> {
    int value;
public:
    CanvasNextPrevReaction(Canvas *canvas, int value) :
    EventAcceptor(canvas),
    value(value)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        acceptor->next_layer(value);

        return EventAccResult::cont;
    }
};

class CanvasNewLayerClickedReaction : public EventAcceptor<Canvas, Event::Clicked> {
public:
    CanvasNewLayerClickedReaction(Canvas *canvas) :
    EventAcceptor(canvas)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        acceptor->new_layer();

        return EventAccResult::cont;
    }
};


void spawn_canvas_window(RedactorEngine *engine, double size_x, double size_y) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("Aboba", {{200, 200}, {size_x, size_y}}, window_style);

    engine->add_view(window);

    auto canvas = new v_Canvas({0, {size_x, size_y}}, engine->visual->get_renderer(), engine->get_tool_manager());

    window->get_content()->add_subview(canvas);

    auto header = window->get_header();
    auto options = header->get_options();

    auto button_prev = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::Arrow::left());
    options->add_subview(button_prev);

    options->add_placehodler(5);

    auto button_plus = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::plus());
    options->add_subview(button_plus);

    options->add_placehodler(5);

    auto button_next = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::Arrow::right());
    options->add_subview(button_next);

    options->normal_stretch();
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
