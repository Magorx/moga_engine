#include <string>


std::string randstr(int len) {
    std::string ret;

    for (int i = 0; i < len; ++i) {
        ret += 'a' + rand() % ('z' - 'a' + 1);
    }

    return ret;
}


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
    v_Window *window;
public:
    CanvasNextPrevReaction(v_Window *window, Canvas *canvas, int value) :
    EventAcceptor(canvas),
    value(value),
    window(window)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        int ret = acceptor->next_layer(value);

        char *cur_label = window->get_header()->get_label_text();
        std::string str = cur_label;
        str[str.size() - 1] = '0' + ret;
        window->get_header()->add_label(str.c_str(), Resources.font.size.basic_header, Resources.font.smart_color.basic_header);

        return EventAccResult::cont;
    }
};

class CanvasNewLayerClickedReaction : public EventAcceptor<Canvas, Event::Clicked> {
    v_Window *window;
public:
    CanvasNewLayerClickedReaction(v_Window *window, Canvas *canvas) :
    EventAcceptor(canvas),
    window(window)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        int ret = acceptor->new_layer();

        char *cur_label = window->get_header()->get_label_text();
        std::string str = cur_label;
        str[str.size() - 1] = '0' + ret;
        window->get_header()->add_label(str.c_str(), Resources.font.size.basic_header, Resources.font.smart_color.basic_header);

        return EventAccResult::cont;
    }
};


class SaveCanvasReaction : public EventReaction<Event::Clicked> {
    v_Window *window;
    Canvas *canvas;

public:
    SaveCanvasReaction(v_Window *window, Canvas *canvas):
    window(window),
    canvas(canvas)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        auto filename = window->get_header()->get_label_text();
        canvas->save_to_file(filename);

        return EventAccResult::none;
    }
};


v_Window *spawn_canvas_window(RedactorEngine *engine, const ViewBody &body) {
    auto window_style = StdStyle::Window::basic();

    auto name = randstr(10) + " - 0";

    auto window = new v_Window(name.c_str(), body, window_style);

    engine->add_view(window);

    auto canvas = new v_Canvas({0, {body.size.x(), body.size.y()}}, engine->visual->get_renderer(), engine->get_tool_manager());

    window->get_content()->add_subview(canvas);

    auto header = window->get_header();
    auto options = header->get_options();

    auto button_prev = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::Arrow::left());
    options->add_subview(button_prev);
    button_prev->e_clicked.add(new CanvasNextPrevReaction(window, canvas->get_canvas(), -1));

    options->add_placehodler(5);

    auto button_plus = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::plus());
    options->add_subview(button_plus);
    button_plus->e_clicked.add(new CanvasNewLayerClickedReaction(window, canvas->get_canvas()));

    options->add_placehodler(5);

    auto button_next = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::Arrow::right());
    options->add_subview(button_next);
    button_next->e_clicked.add(new CanvasNextPrevReaction(window, canvas->get_canvas(), +1));

    options->add_placehodler(5);

    auto button_save = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::save());
    options->add_subview(button_save);
    button_save->e_clicked.add(new SaveCanvasReaction(window, canvas->get_canvas()));

    options->normal_stretch();

    return window;
}

v_Window *spawn_color_picker_window(RedactorEngine *engine, const ViewBody &body) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("ColorPuker", {body.position, {body.size.x(), body.size.y()}}, window_style);

    engine->add_view(window);

    auto picker = new v_ColorPicker({0, {body.size.x(), body.size.y()}}, engine->get_tool_manager());

    window->add_subview(picker);

    return window;
}


class AddNewCanvasReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

public:
    AddNewCanvasReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        spawn_canvas_window(engine, {engine->random_screen_pos(), {300, 200}});

        return EventAccResult::none;
    }
};

class AddNewColorPickerReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

public:
    AddNewColorPickerReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        spawn_color_picker_window(engine, {engine->random_screen_pos(), {200, 200}});

        return EventAccResult::none;
    }
};
