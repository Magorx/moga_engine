class SpawnBallLambda : public EventReaction<Event::MousePress> {
    MogaEngine *engine;

public:
    SpawnBallLambda(MogaEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MousePress &) override {
        gen_ball(engine);
        return EventAccResult::done;
    }
};


class SwitchOnChemistryModeLambda : public EventReaction<Event::MousePress> {
    ChemEngine *engine;

public:
    SwitchOnChemistryModeLambda(ChemEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MousePress &) override {
        engine->enable_chemistry();
        return EventAccResult::done;
    }
};

class SwitchOffChemistryModeLambda : public EventReaction<Event::MousePress> {
    ChemEngine *engine;

public:
    SwitchOffChemistryModeLambda(ChemEngine *engine):
    engine(engine)
    {}

    EventAccResult operator()(const Event::MousePress &) override {
        engine->disable_chemistry();
        return EventAccResult::done;
    }
};
