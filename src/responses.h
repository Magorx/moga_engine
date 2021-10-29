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


// class ToogleChemistryModelLambda : public EventReaction<Event::Activator> {
//     ChemEngine *engine;

// public:
//     ToogleChemistryModelLambda(ChemEngine *engine):
//     engine(engine)
//     {}

//     EventAccResult operator()(const Event::Activator &, const EventAccResult*) override {
//         engine->toggle_chemistry();
//         return EventAccResult::none;
//     }
// };

// class PressChemistryModelToggler : public EventReaction<Event::MousePress> {
//     ChemEngine *engine;
//     Event::Activator::State mode;

// public:

//     PressChemistryModelToggler(ChemEngine *engine, Event::Activator::State mode):
//     engine(engine),
//     mode(mode)
//     {}

//     EventAccResult operator()(const Event::MousePress &, const EventAccResult*) override {
//         if (mode == Event::Activator::State::off) {
//             engine->chemistry_off();
//         } else if (mode == Event::Activator::State::on) {
//             engine->chemistry_on();
//         } else {
//             engine->toggle_chemistry();
//         }
//         return EventAccResult::done;
//     }
// };

// class SetPhysTimeMultiplier : public EventReaction<Event::MousePress> {
//     MogaEngine *engine;
//     double mult;

// public:

//     SetPhysTimeMultiplier(MogaEngine *engine, double mult):
//     engine(engine),
//     mult(mult)
//     {}

//     EventAccResult operator()(const Event::MousePress &, const EventAccResult*) override {
//         engine->set_phys_time_mult(mult);
//         return EventAccResult::done;
//     }
// };


class AddNewCanvasReaction : public EventReaction<Event::MouseRelease> {
    MogaEngine *engine;

public:
    AddNewCanvasReaction(MogaEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MouseRelease &, const EventAccResult*) override {

        auto window_style = StdStyle::Window::basic();

        auto window = new v_Window("Aboba", {200, 200}, window_style);

        engine->add_view(window);

        return EventAccResult::none;
    }
};
