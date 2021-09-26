#include "chemistry/chem_engine.h"

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

void generate_balls(MogaEngine *eng, int cnt = 60) {
    for (int i = 0; i < cnt; ++i) {
        Vec3d col = Vec3d::random_unit() * 250;
        SmartColor *color = nullptr;
        color = new SmartColor(col);
        
        Object *ball = new o_Ball({(double) (rand() % 500) + 150, (double)(rand() % 300) + 120}, 5, color, 1);
        eng->add_object(ball);

        Vec3d vel = Vec3d::random_unit() * 200;
        vel.set(2, 0);
        ball->get_solid_body()->set_velocity(vel);
    }

    auto color = new SmartColorSin(Color(255, 255, 255), 2);
    eng->add_tickable(color);

    Object *ball = new o_Ball({500, 300}, 30, color, 6);
    eng->add_object(ball);
}

int main() {
    srand(time(NULL));
    ChemEngine moga("MOGA", SCR_W, SCR_H, 1);
    sf::RenderWindow *window = moga.visual->get_renderer()->get_window();

    SmartColor *color = new SmartColorSin(Color{40, 230, 150});
    moga.add_tickable(color);

    create_cage(&moga, color);
    generate_balls(&moga);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        moga.tick();
    }

	return 0;
}
