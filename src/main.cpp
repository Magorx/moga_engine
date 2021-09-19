// #define SHOW_UHHH_COLLISION
// #define HIDE_APPROXIMATION_RENDER
// #define PRINT_PHASE_TICK

#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "MogaEngine.h"

const int SCR_W    = 800;
const int SCR_H    = 600;

int main() { // magic numbers are allowed here for scene components
    srand(time(NULL));

    VisualEngine veng("MOGA", SCR_W, SCR_H);
    sf::RenderWindow *window = veng.get_renderer()->get_window();

    SmartColor *color = new SmartColor({150, 170, 90});
    r_Circle *circle = new r_Circle({150, 40}, 30, color);

    veng.add_renderable(circle);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(*window));
        circle->pos = {mouse_pos.x, mouse_pos.y};

        veng.tick();
    }

	return 0;
}
