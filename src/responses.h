#include <string>
#include "utils/logger.h"


std::string randstr(int len) {
    std::string ret;

    for (int i = 0; i < len; ++i) {
        ret += 'a' + rand() % ('z' - 'a' + 1);
    }

    return ret;
}


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


class CanvasOpenEffects : public EventAcceptor<Canvas, Event::Clicked> {
    MogaEngine *engine;
    std::map<Layer*, eff_RGBSplined*> effects;
    std::map<Layer*, v_Window*> windows;

public:
    CanvasOpenEffects(MogaEngine *engine, Canvas *canvas, AbstractView *new_layer_button);
    void update_effects(eff_RGBSplined *effect);
    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override;
};

class CanvasOpenEffectsUpdater : public EventAcceptor<CanvasOpenEffects, Event::DataPtr> {
public:
    CanvasOpenEffectsUpdater(CanvasOpenEffects *acceptor) : EventAcceptor(acceptor) {}

    EventAccResult operator()(const Event::DataPtr &event, const EventAccResult*) override {
        if (!event.check_receiver(this)) return EventAccResult::none;

        acceptor->update_effects((eff_RGBSplined*) event.data);

        return EventAccResult::cont;
    }
};

CanvasOpenEffects::CanvasOpenEffects(MogaEngine *engine, Canvas *canvas, AbstractView *new_layer_button) :
EventAcceptor(canvas),
engine(engine)
{
    new_layer_button->e_data_ptr.add(new CanvasOpenEffectsUpdater(this));
}

void CanvasOpenEffects::update_effects(eff_RGBSplined *effect) {
    auto layer = acceptor->get_active_layer();
    effects[layer] = effect;
    auto window = effect->create_settings_window(engine);
    windows[layer] = window;

    window->get_header()->get_button_hide()->e_toggle_activity.emit({});

    window->get_engine()->add_view(window);
}

EventAccResult CanvasOpenEffects::operator()(const Event::Clicked &, const EventAccResult*) {
    windows[acceptor->get_active_layer()]->get_header()->get_button_hide()->e_toggle_activity.emit({});

    return EventAccResult::cont;
}

class CanvasNewLayerClickedReaction : public EventAcceptor<Canvas, Event::Clicked> {
    v_Window *window;
    CanvasOpenEffectsUpdater *effects_opener_upd;
public:
    CanvasNewLayerClickedReaction(v_Window *window, Canvas *canvas, CanvasOpenEffectsUpdater *effects_opener_upd) :
    EventAcceptor(canvas),
    window(window),
    effects_opener_upd(effects_opener_upd)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        int ret = acceptor->new_layer();

        char *cur_label = window->get_header()->get_label_text();
        std::string str = cur_label;
        str[str.size() - 1] = '0' + ret;
        window->get_header()->add_label(str.c_str(), Resources.font.size.basic_header, Resources.font.smart_color.basic_header);

        auto effect = new eff_RGBSplined(acceptor->get_active_layer());
        acceptor->get_active_layer()->add_effect(effect);

        // auto settings_window = effect->create_settings_window(window->get_engine());
        // settings_window->get_header()->get_button_hide()->e_toggle_activity.emit({});

        window->e_data_ptr.emit({effects_opener_upd, effect});
        printf("emited\n");

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


class CreateOrActivateWindow : public EventReaction<Event::Clicked> {
    v_Window *window;
    Canvas *canvas;

public:
    CreateOrActivateWindow(v_Window *window, Canvas *canvas):
    window(window),
    canvas(canvas)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        auto filename = window->get_header()->get_label_text();
        canvas->save_to_file(filename);

        return EventAccResult::none;
    }
};


v_Window *spawn_canvas_window(RedactorEngine *engine, const ViewBody &body, Canvas **out_canvas = nullptr) {
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

    auto button_open_effects = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::rgb());
    auto effects_opener = new CanvasOpenEffects(engine, canvas->get_canvas(), window);
    button_open_effects->e_clicked.add(effects_opener);

    options->add_subview(button_plus);
    auto updater = new CanvasOpenEffectsUpdater(effects_opener);
    window->e_data_ptr.add(updater);
    button_plus->e_clicked.add(new CanvasNewLayerClickedReaction(window, canvas->get_canvas(), updater));

    options->add_placehodler(5);

    auto button_next = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::Arrow::right());
    options->add_subview(button_next);
    button_next->e_clicked.add(new CanvasNextPrevReaction(window, canvas->get_canvas(), +1));

    options->add_placehodler(5);

    auto button_save = new v_Button({0, {PX_UTIL_BUTTON_SIZE + 5, PX_UTIL_BUTTON_SIZE}}, StdStyle::Button::save());
    options->add_subview(button_save);
    button_save->e_clicked.add(new SaveCanvasReaction(window, canvas->get_canvas()));

    options->add_subview(button_open_effects);

    options->normal_stretch();

    if (out_canvas) {
        *out_canvas = canvas->get_canvas();
    }

    auto effect = new eff_RGBSplined(canvas->get_canvas()->get_active_layer());
    canvas->get_canvas()->get_active_layer()->add_effect(effect);

    effects_opener->update_effects(effect);

    return window;
}

v_Window *spawn_color_picker_window(RedactorEngine *engine, const ViewBody &body) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("ColorPicker", {body.position, {body.size.x(), body.size.y()}}, window_style);

    engine->add_view(window);

    auto picker = new v_ColorPicker({0, {body.size.x(), body.size.y()}}, engine->get_tool_manager());

    window->add_subview(picker);

    return window;
}


v_Window *open_image(RedactorEngine *engine, const Vec2d &pos) {
    char filename[200] = "1.jpg";
    // scanf("%200s", filename);

    RImage img;
    if (!img.loadFromFile(filename)) {
        logger.error("open_image", "can't open image %s", filename);
        return nullptr;
    }

    auto size = img.getSize();

    Canvas *canvas;
    v_Window *window = spawn_canvas_window(engine, {pos, {(double) size.x, (double) size.y}}, &canvas);
    if (!window) {
        logger.error("open_image", "can't create canvas for image %s [%lux%lu]", filename, size.x, size.y);
        return nullptr;
    }

    RTexture texture;
    texture.loadFromImage(img);
    canvas->get_active_layer()->copy_from(&texture);

    return window;
}


class AddNewCanvasReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

public:
    AddNewCanvasReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        spawn_canvas_window(engine, {engine->random_screen_pos(), {400, 300}});

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

class OpenImage : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

public:
    OpenImage(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        open_image(engine, engine->random_screen_pos());

        return EventAccResult::none;
    }
};
