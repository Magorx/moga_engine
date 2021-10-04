class SpawnBallLambda : public Lambda {
    MogaEngine *engine;

public:
    SpawnBallLambda(MogaEngine *engine):
    engine(engine)
    {}

    void operator()() {
        gen_ball(engine);
    }
};


class SwitchOnChemistryModeLambda : public Lambda {
    ChemEngine *engine;

public:
    SwitchOnChemistryModeLambda(ChemEngine *engine):
    engine(engine)
    {}

    void operator()() {
        engine->enable_chemistry();
    }
};

class SwitchOffChemistryModeLambda : public Lambda {
    ChemEngine *engine;

public:
    SwitchOffChemistryModeLambda(ChemEngine *engine):
    engine(engine)
    {}

    void operator()() {
        engine->disable_chemistry();
    }
};
