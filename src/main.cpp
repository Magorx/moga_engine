#include "MogaEngine.h"

const int SCR_W  = 800;
const int SCR_H  = 600;

int main() { // magic numbers are allowed here for scene components
    MogaEngine moga("MOGA", SCR_W, SCR_H, 1);
    sf::RenderWindow *window = moga.visual->get_renderer()->get_window();

    SmartColor *color = new SmartColorSin(Color{150, 170, 90}, 1.2);
    Object *ball = new o_Ball({150, 40}, 20, color);

    moga.add_object(ball);
    
    ball->get_solid_body()->apply_impulse({10, 2});

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
