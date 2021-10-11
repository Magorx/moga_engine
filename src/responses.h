class SpawnBallLambda : public EventReaction<Event::MousePress> {
    MogaEngine *engine;

public:
    SpawnBallLambda(MogaEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MousePress &) override {
        gen_ball(engine);
        return EventAccResult::none;
    }
};


class ToogleChemistryModelLambda : public EventReaction<Event::Toggle> {
    ChemEngine *engine;

public:
    ToogleChemistryModelLambda(ChemEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::Toggle &) override {
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
