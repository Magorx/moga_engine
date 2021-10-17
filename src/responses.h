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

public:
    PressChemistryModelToggler(ChemEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MousePress &, const EventAccResult*) override {
        engine->toggle_chemistry();
        return EventAccResult::none;
    }
};


template <typename T>
class EventCatcher : public EventReaction<T> {
public:
    EventAccResult operator()(const T &) override {
        return EventAccResult::done;
    }
};
