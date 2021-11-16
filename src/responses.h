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
        window->get_header()->add_label(str.c_str(), Resources.font.size.basic_header, Resources.font.color.basic_header);

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
    window->focus();

    window->get_engine()->add_view(window);
}

EventAccResult CanvasOpenEffects::operator()(const Event::Clicked &, const EventAccResult*) {
    windows[acceptor->get_active_layer()]->get_header()->get_button_hide()->e_toggle_activity.emit({});
    windows[acceptor->get_active_layer()]->focus();

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
        window->get_header()->add_label(str.c_str(), Resources.font.size.basic_header, Resources.font.color.basic_header);

        auto effect = new eff_RGBSplined(acceptor->get_active_layer());
        acceptor->get_active_layer()->add_effect(effect);

        // auto settings_window = effect->create_settings_window(window->get_engine());
        // settings_window->get_header()->get_button_hide()->e_toggle_activity.emit({});

        window->e_data_ptr.emit({effects_opener_upd, effect});

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
        filename[strlen(filename) - 4] = '\0';
        canvas->save_to_file(filename);

        logger.info("moga", "canvas \"%s.png\" was saved", filename);
        filename[strlen(filename)] = ' ';


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


v_Window *spawn_canvas_window(RedactorEngine *engine, const ViewBody &body, Canvas **out_canvas = nullptr, char *name = nullptr) {
    auto window_style = StdStyle::Window::basic();

    std::string canvas_name;
    if (name) {
        canvas_name = name;
    } else {
        canvas_name = randstr(10);
    }

    canvas_name += " - 0";

    auto window = new v_Window(canvas_name.c_str(), body, window_style);

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


class NewCanvasReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

    double width;
    double height;
    char *name;

    Canvas **out_canvas = nullptr;

public:
    NewCanvasReaction(RedactorEngine *engine, Canvas **out_canvas = nullptr):
    engine(engine),
    width(0),
    height(0),
    name(nullptr),
    out_canvas(out_canvas)
    {}

    double *get_width_ptr() { return &width; }
    double *get_height_ptr() { return &height; }
    char **get_name_ptr() { return &name; }

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        if (!(width >= 1 && height >= 1 && name && *name != '\0')) {
            auto error_window = v_DialogWindow::Error(200, "Bad parametrs");
            engine->add_view(error_window);
            return EventAccResult::cont;
        }

        ViewBody body = {engine->random_screen_pos(), {width, height}};

        spawn_canvas_window(engine, body, out_canvas, name);

        return EventAccResult::cont;
    }
};


v_Window *spawn_canvas_window_dialog(RedactorEngine *engine) {
    auto dw = new v_DialogWindow("New canvas", 250, 5, -30);
    
    auto f_width  = dw->add_field("Width", 100, "px");
    auto f_height = dw->add_field("Height", 100, "px");
    auto f_name   = dw->add_field("Name");
    auto b_create = dw->add_accept_button("Create");

    auto reaction = new NewCanvasReaction(engine);
    b_create->e_clicked.add(reaction);

    f_width->e_text_changed.add (new TextFieldChangeValueSynchronizer(reaction->get_width_ptr()));
    f_height->e_text_changed.add(new TextFieldChangeValueSynchronizer(reaction->get_height_ptr()));
    f_name->e_text_changed.add  (new TextFieldChangeStringSynchronizer(reaction->get_name_ptr()));

    dw->make_closing_field(f_width,  b_create);
    dw->make_closing_field(f_height, b_create);
    dw->make_closing_field(f_name,   b_create);
    dw->make_closing_button(b_create);

    f_width->set_string("800");
    f_height->set_string("600");
    f_name->set_string(randstr(10).c_str());

    engine->add_view(dw);

    dw->select_first_field();

    return dw;
}

v_Window *spawn_color_picker_window(RedactorEngine *engine, const ViewBody &body) {
    auto window_style = StdStyle::Window::basic();

    auto window = new v_Window("ColorPicker", {body.position, {body.size.x(), body.size.y()}}, window_style);

    engine->add_view(window);

    auto picker = new v_ColorPicker({0, {body.size.x(), body.size.y()}}, engine->get_tool_manager());

    window->add_subview(picker);

    return window;
}


class OpenCanvasWithImageReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;
    char *path;

public:
    OpenCanvasWithImageReaction(RedactorEngine *engine):
    engine(engine),
    path(nullptr)
    {}

    char **get_path_ptr() { return &path; }

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        RImage img;
        if (!img.loadFromFile(path)) {
            logger.warning("moga", "can't open image %s", path);

            auto error_window = v_DialogWindow::Error(200, "Image doesn't exist");
            engine->add_view(error_window);

            return EventAccResult::none;
        }

        auto size = img.getSize();

        Canvas *canvas;
        v_Window *window = spawn_canvas_window(engine, {engine->random_screen_pos(), {(double) size.x, (double) size.y}}, &canvas);
        if (!window) {
            logger.error("moga", "can't create canvas for image %s [%lux%lu]", path, size.x, size.y);
            return EventAccResult::none;
        }

        RTexture texture;
        texture.loadFromImage(img);
        canvas->get_active_layer()->copy_from(&texture);

        return EventAccResult::none;
    }
};


v_Window *spawn_open_image_dialog(RedactorEngine *engine) {
    auto dw = new v_DialogWindow("Open image", 200);
    engine->add_view(dw);

    auto path = dw->add_field("Path");
    auto open_button = dw->add_accept_button("Open");

    auto open_reaction = new OpenCanvasWithImageReaction(engine);
    open_button->e_clicked.add(open_reaction);

    path->e_text_changed.add(new TextFieldChangeStringSynchronizer(open_reaction->get_path_ptr()));

    dw->make_closing_field(path, open_button);

    dw->select_first_field();

    return dw;
}


class SetActiveTool : public EventReaction<Event::Clicked> {
    ToolManager *tool_manager;
    int idx;

public:
    SetActiveTool(ToolManager *tool_manager, int idx):
    tool_manager(tool_manager),
    idx(idx)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {
        tool_manager->set_active_tool(idx);

        return EventAccResult::none;
    }
};


class ToolManagerHotkeys : public EventAcceptor<ToolManager, Event::KeyDown> {
    int prev_index = 0;

public:
    ToolManagerHotkeys(ToolManager *tool_manager) :
    EventAcceptor(tool_manager)
    {}

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult*) override {
        int cur = acceptor->get_active_tool_idx();

        switch (event.code) {
            case Keyboard::Key::b:
                if (cur == TOOL_BRUSH_IDX) break;

                prev_index = cur;
                acceptor->set_active_tool(TOOL_BRUSH_IDX);
                break;
            
            case Keyboard::Key::e:
                if (cur == TOOL_ERASER_IDX) {
                    acceptor->set_draw_color({0, 0, 0, 0});
                } else {
                    prev_index = cur;
                    acceptor->set_active_tool(TOOL_ERASER_IDX);
                }
                break;
            
            case Keyboard::Key::q:
                if (cur == TOOL_PIPETTE_IDX) break;
                prev_index = cur;
                acceptor->set_active_tool(TOOL_PIPETTE_IDX);
                break;
            
            case Keyboard::Key::x:
                acceptor->set_active_tool(prev_index);
                prev_index = cur;
                break;
            
            default:
                break;
        }

        return EventAccResult::cont;
    }
};


class ToolManagerSetToolSize : public EventAcceptor<ToolManager, Event::FractionChanged> {
    double max_rad;

public:
    ToolManagerSetToolSize(ToolManager *tool_manager, double max_rad = 35) :
    EventAcceptor(tool_manager),
    max_rad(max_rad)
    {}

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult*) override {
        acceptor->set_size(1 + max_rad * event.fraction.x());
        acceptor->update_active_tool();

        return EventAccResult::cont;
    }
};


class ToolManagerScrollShiftToolSize : public EventAcceptor<v_Magnetic, Event::Scroll> {
    double granularity;

public:
    ToolManagerScrollShiftToolSize(v_Magnetic *slider, double granularity = -0.015) :
    EventAcceptor(slider),
    granularity(granularity)
    {}

    EventAccResult operator()(const Event::Scroll &event, const EventAccResult*) override {
        acceptor->shift_fraction({event.delta.y() * granularity, 0});

        return EventAccResult::done;
    }
};


v_Window *spawn_tool_picker_window(RedactorEngine *engine, const ViewBody &body) {
    auto window = new v_Window("Tools", body);

    engine->add_view(window);

    v_VerticalLayout *layout = new v_VerticalLayout({0, body.size}, {0.05, 0.95}, 5);
    window->get_content()->add_subview(layout);

    window->e_key_down.add(new ToolManagerHotkeys(engine->get_tool_manager()));

    v_Button *b_brush   = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    v_Button *b_eraser  = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    v_Button *b_pipette = new v_Button({0, 0}, StdStyle::Button::basic_menu());

    layout->layout_add(b_brush,  3);
    layout->layout_add(b_eraser, 3);
    layout->layout_add(b_pipette, 3);

    auto slider = new v_Magnetic({0, 0}, {0, PX_MAG_DOT});
    layout->layout_add(slider);

    slider->set_fraction({0, 0.50});
    slider->toggle_x_restriction();
    slider->set_appearence(Resources.add_appr(new AppearenceColor({180, 160, 190})));
    slider->get_dot()->set_appearence(Resources.add_appr(new AppearenceTexture(Resources.texture.stick, {1, 1}, -slider->get_dot()->get_body().size / 2)));

    b_brush->add_label("[b] Bruh", Resources.font.size.basic_menu, Resources.font.color.basic_menu);
    b_eraser->add_label("[e] Eraer", Resources.font.size.basic_menu, Resources.font.color.basic_menu);
    b_pipette->add_label("[q] Pipete", Resources.font.size.basic_menu, Resources.font.color.basic_menu);

    b_brush->e_clicked.add(new SetActiveTool(engine->get_tool_manager(), 0));
    b_eraser->e_clicked.add(new SetActiveTool(engine->get_tool_manager(), 1));
    b_pipette->e_clicked.add(new SetActiveTool(engine->get_tool_manager(), 2));

    slider->e_fraction_changed.add(new ToolManagerSetToolSize(engine->get_tool_manager()));
    slider->e_scroll.add(new ToolManagerScrollShiftToolSize(slider));

    return window;
}

class AddNewCanvasReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

public:
    AddNewCanvasReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        spawn_canvas_window_dialog(engine);

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

        spawn_color_picker_window(engine, {engine->random_screen_pos(), {200, 240}});

        return EventAccResult::none;
    }
};

class AddNewToolManagerWindowReaction : public EventReaction<Event::Clicked> {
    RedactorEngine *engine;

public:
    AddNewToolManagerWindowReaction(RedactorEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Clicked &, const EventAccResult*) override {

        spawn_tool_picker_window(engine, {engine->random_screen_pos(), {200, 200}});

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

        spawn_open_image_dialog(engine);

        return EventAccResult::none;
    }
};

class DebugTextChange : public EventReaction<Event::TextChanged> {
public:
    DebugTextChange()
    {}

    EventAccResult operator()(const Event::TextChanged &event, const EventAccResult*) override {

        printf("TextChanged:\n");
        printf("  str  : [%s]\n", event.text);
        printf("  val_d: [%.20lg]\n", event.val_d);
        printf("  val_i: [%d]\n", event.val_i);

        return EventAccResult::none;
    }
};

