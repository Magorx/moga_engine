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


v_Window *spawn_canvas_window(RedactorEngine *engine, const ViewBody &body) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("Aboba", body, window_style);

    engine->add_view(window);

    auto canvas = new v_Canvas({0, {body.size.x(), body.size.y()}}, engine->visual->get_renderer(), engine->get_tool_manager());

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

    return window;
}

v_Window *spawn_color_picker_window(RedactorEngine *engine, const ViewBody &body) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("", {{200, 200}, {body.size.x(), body.size.y()}}, window_style);

    v_Highlighter *lb = new v_Highlighter({0, window->get_header()->get_options()->get_body().size});
    window->get_header()->get_options()->add_subview(lb);
    lb->add_label("Color Puker", Resources.font.size.basic_header, Resources.font.smart_color.basic_header);

    engine->add_view(window);

    auto picker = new v_ColorPicker({0, {body.size.x(), body.size.y()}}, engine->get_tool_manager());

    window->add_subview(picker);

    return window;
}


class AddNewCanvasReaction : public EventReaction<Event::MouseRelease> {
    RedactorEngine *engine;

public:
    AddNewCanvasReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MouseRelease &, const EventAccResult*) override {

        spawn_canvas_window(engine, {200, {300, 200}});

        return EventAccResult::none;
    }
};
