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


class ToogleChemistryModelLambda : public EventReaction<Event::Activator> {
    ChemEngine *engine;

public:
    ToogleChemistryModelLambda(ChemEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Activator &, const EventAccResult*) override {
        engine->toggle_chemistry();
        return EventAccResult::none;
    }
};

class PressChemistryModelToggler : public EventReaction<Event::MousePress> {
    ChemEngine *engine;
    Event::Activator::State mode;

public:

    PressChemistryModelToggler(ChemEngine *engine, Event::Activator::State mode):
    engine(engine),
    mode(mode)
    {}

    EventAccResult operator()(const Event::MousePress &, const EventAccResult*) override {
        if (mode == Event::Activator::State::off) {
            engine->chemistry_off();
        } else if (mode == Event::Activator::State::on) {
            engine->chemistry_on();
        } else {
            engine->toggle_chemistry();
        }
        return EventAccResult::done;
    }
};

class SetPhysTimeMultiplier : public EventReaction<Event::MousePress> {
    MogaEngine *engine;
    double mult;

public:

    SetPhysTimeMultiplier(MogaEngine *engine, double mult):
    engine(engine),
    mult(mult)
    {}

    EventAccResult operator()(const Event::MousePress &, const EventAccResult*) override {
        engine->set_phys_time_mult(mult);
        return EventAccResult::done;
    }
};


class AddNewCanvasReaction : public EventReaction<Event::MouseRelease> {
    MogaEngine *engine;

public:
    AddNewCanvasReaction(MogaEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MouseRelease &, const EventAccResult*) override {

        // WindowStyle *

        // v_Window *window = new v_Window("window", {400, 400}, 35, Resources.texture.window.basic);
        // window->get_body().position = {50, 50};
        // engine->add_view(window);

        // auto accessory = window->get_accessory();

        // auto anima_idle = new AppearenceAnimation(&Resources.animation.lightning_idle.frames, 0.13, true);
        // auto anima_hover = new AppearenceAnimation(&Resources.animation.lightning_hover.frames, 0.09);
        // engine->add_tickable(anima_idle);
        // engine->add_tickable(anima_hover);
        // anima_idle->start();
        // anima_hover->start();
        
        // accessory->set_appearence(anima_idle);

        // accessory->e_mouse_move.add(new AVAnimatorMove(accessory, anima_hover, anima_idle));

        return EventAccResult::none;
    }
};