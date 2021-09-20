#include "MogaEngine.h"

const int SCR_W  = 800;
const int SCR_H  = 600;

void create_cage(MogaEngine *eng, SmartColor *color) {
    o_Line *line_1 = new o_Line({100, 100}, 
                                {700, 50}, 
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
}

void generate_balls(MogaEngine *eng, int cnt = 100) {
    for (int i = 0; i < cnt; ++i) {
        Vec3d col = Vec3d::random_unit() * 250;
        SmartColor *color = nullptr;
        color = new SmartColor(col);
        
        Object *ball = new o_Ball({(double) (rand() % 500) + 150, (double)(rand() % 300) + 120}, 10, color);
        eng->add_object(ball);

        Vec3d pulse = Vec3d::random_unit().abs() * 100 + Vec3d(30, 30);
        pulse.set(2, 0);
        ball->get_solid_body()->apply_impulse(pulse);
    }

    auto color = new SmartColor(Color(255, 255, 255));
    Object *ball = new o_Ball({500, 300}, 30, color, 100);
    eng->add_object(ball);
}

int main() { // magic numbers are allowed here for scene components
    srand(time(NULL));
    MogaEngine moga("MOGA", SCR_W, SCR_H, 1);
    sf::RenderWindow *window = moga.visual->get_renderer()->get_window();

    SmartColor *color = new SmartColor(Color{230, 120, 190});

    create_cage(&moga, color);
    generate_balls(&moga);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        // sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(*window));

        moga.tick();
    }

	return 0;
}
