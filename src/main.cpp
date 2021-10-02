#include "chemistry/chem_engine.h"

#include "view/view.h"

const int SCR_W  = 800;
const int SCR_H  = 600;

void create_cage(MogaEngine *eng, SmartColor *color) {
    o_Line *line_1 = new o_Line({100, 100}, 
                                {350, 0}, 
                                color);

    o_Line *line_11 = new o_Line({350, 0}, 
                                 {700, 100}, 
                                 color);
    
    o_Line *line_2 = new o_Line({100, 100}, 
                                {100, 500}, 
                                color);
    
    o_Line *line_3 = new o_Line({700, 100}, 
                                {700, 500},
                                color);
    
    o_Line *line_4 = new o_Line({100, 500}, 
                                {700, 500}, 
                                color);

    eng->add_object(line_1);
    eng->add_object(line_2);
    eng->add_object(line_3);
    eng->add_object(line_4);
    eng->add_object(line_11);
}

void gen_ball(MogaEngine *eng) {
    Vec3d col = Vec3d::random_unit() * 250;
    SmartColor *color = nullptr;
    color = new SmartColor(col);
    
    Object *ball = new o_Ball({(double) (rand() % 500) + 150, (double)(rand() % 300) + 120}, 5, color, 1);
    eng->add_object(ball);
    eng->add_tickable(color);

    Vec3d vel = Vec3d::random_unit() * 200;
    vel.set(2, 0);
    ball->get_solid_body()->set_velocity(vel);
}

void generate_balls(MogaEngine *eng, int cnt = 100) {
    for (int i = 0; i < cnt; ++i) {
        gen_ball(eng);
    }

    auto color = new SmartColorSin(Color(255, 255, 255), 2);
    eng->add_tickable(color);

    Object *ball = new o_Ball({500, 300}, 30, color, 6);
    eng->add_object(ball);
}


class SpawnBallMouseLambda : public MouseLambda {
    MogaEngine *engine;

public:
    SpawnBallMouseLambda(MogaEngine *engine):
    engine(engine)
    {}

    void operator()(const Vec2d &) override {
        gen_ball(engine);
    }
};


int main() {
    srand(time(NULL));
    ChemEngine moga("MOGA", SCR_W, SCR_H, 1);

    SmartColor *color = new SmartColorSin(Color{40, 230, 150});
    moga.add_tickable(color);

    create_cage(&moga, color);
    generate_balls(&moga, 5);

    SmartColor *col = new SmartColor({155, 135, 100});
    v_Button *butt = new v_Button(ViewBody{{100, 50}, {75, 200}}, col);
    moga.add_tickable(col);
    moga.add_view(butt);

    butt->bind(new SpawnBallMouseLambda(&moga));

    moga.everlasting_loop();

	return 0;
}
