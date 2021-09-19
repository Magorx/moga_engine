#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>

#include "kctf_engine_settings.hpp"
#include "../physics/physics_engine.hpp"
#include "../visual/visual_engine.hpp"
#include "objects_collection.hpp"

#include <vector>

class KCTF_Engine
{
	VisualEngine  visual;
	PhysicsEngine physics;

	std::vector<Object*> objects;

	double current_time;
	double dt;
//  ===== SFML WORK =====
	sf::Clock clock;

	double init_time;
	double prev_frame_time;
	double last_frame_duration;
    double fps_second_start;
    double current_physics_time;
    double cur_time;
    
    int frames_cnt;
    int fps;
    int second_passed;
    int pause_mode;

    sf::RenderWindow window;
    sf::Sprite scaling_sprite;
    sf::Texture scaling_texture;

    sf::Vector2i absolute_mouse_pos;
    Vec2d 		 mouse_pos;
//  =====================

	void frame_init_tick() {
		cur_time = clock.getElapsedTime().asSeconds();
        last_frame_duration = cur_time - prev_frame_time;
        prev_frame_time = cur_time;

    	sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

           	if (event.type == sf::Event::KeyPressed) {
           		if (event.key.code == sf::Keyboard::Space) {
           			if (pause_mode) {
           				pause_mode = 0;
           			} else {
           				pause_mode = 1;
           			}
           		}
           	}

           	if (event.type == sf::Event::KeyPressed) {
           		if (event.key.code == sf::Keyboard::Enter) {
           			pause_mode = ONE_FRAME;
           		}
           	}

           	if (event.type == sf::Event::MouseButtonPressed) {
			    obj_Box *box = new obj_Box(10, {30, 30}, {120,  120, 120}, true, NO_FILL, mouse_pos);
			    add_object(box, true);
			}
        }

        current_time  = cur_time - init_time;
        dt = last_frame_duration;

        absolute_mouse_pos = sf::Mouse::getPosition() - window.getPosition();
        mouse_pos = Vec2d(absolute_mouse_pos.x / visual.pixel_size, absolute_mouse_pos.y / visual.pixel_size) + Vec2d(0, -12) * visual.pixel_size / 1.3;

        if (!pause_mode) {
	        second_passed = false;
			double fps_seconds_elapsed = cur_time - fps_second_start;
	        if (fps_seconds_elapsed >= 1.0) {
	        	second_passed = true;
	            if (frames_cnt < CRITICAL_FPS_RATE) {
	                pause_mode = 1;
	            }

	            window.setTitle("KCTF | FPS: " + std::to_string(frames_cnt));
	            fps = frames_cnt;
	            fps_second_start = cur_time;
	            frames_cnt = 0;
	        }
	    }
	}

	void visual_render_tick() {
		#ifdef PRINT_PHASE_TICK
		printf("[visual_tick] %lg\n", current_time);
		#endif
		visual.render(current_time);
        // visual.flush_to_texture(scaling_texture);
        // window.draw(scaling_sprite);
        // window.display();
	}

	void physics_tick() {
		#ifdef PRINT_PHASE_TICK
		printf("[physics_tick] %lg\n", current_physics_time);
		#endif
		physics.tick(current_physics_time, PHYSICS_ENGINE_TIME_STAMP, visual.pmap);
	}

	void logic_tick() {
		#ifdef PRINT_PHASE_TICK
		printf("[logic_tick] %lg\n", current_time);
		#endif
		for (size_t i = 0; i < objects.size(); ++i) {
			objects[i]->logic_tick(current_time, dt);
		}
	}

	void screen_tick() {
		#ifdef PRINT_PHASE_TICK
		printf("[screen_tick] %lg\n", current_time);
		#endif
		visual.flush_to_texture(scaling_texture);
        window.draw(scaling_sprite);
        window.display();
	}

public:

	KCTF_Engine(const size_t screen_width,
				const size_t screen_height,
				const size_t pixel_size):
	visual(screen_height, screen_height, pixel_size),
	physics(),
	objects(),

	current_time(0),
	fps(0),
	second_passed(0),
	dt(0),

	clock(),
	init_time(0),
	prev_frame_time(0),
	last_frame_duration(0),
	fps_second_start(0),
	current_physics_time(0),
	cur_time(0),
	
	frames_cnt(0),
	pause_mode(0),

	window(sf::VideoMode(screen_width * pixel_size, screen_height * pixel_size), "KCTF | FPS: "),
	scaling_sprite(),
	scaling_texture()
	{
		clock.restart();
		init_time            = clock.getElapsedTime().asSeconds();
		prev_frame_time      = init_time;
		last_frame_duration  = init_time;
    	fps_second_start     = init_time;
    	cur_time             = init_time;
    	current_physics_time = init_time;

    	scaling_sprite.setPosition(0, 0);
    	scaling_texture.create(screen_width, screen_height);
	    scaling_sprite.setTexture(scaling_texture);
	    scaling_sprite.setScale(pixel_size, pixel_size);
	}

	bool add_physics_tickable(Tickable *object) {
		return physics.add_tickable(object);
	}

	bool add_physics_collidable(Collidable *object) {
		return physics.add_collidable(object);
	}

	bool add_visual_renderable(Renderable *object) {
		return visual.add_renderable(object);
	}

	bool add_object(Object *object, bool is_collidable = false) {
		if (!object) {
			return false;
		}

		objects.push_back(object);
		add_physics_tickable(object);
		add_visual_renderable(object);

		object->update_approximation(current_time);

		if (is_collidable) {			
			add_physics_collidable(object);
		}

		object->set_engine(*this);

		return true;
	}

	bool tick() {
		if (!window.isOpen()) {
			return false;
		}

		frame_init_tick();

		if (pause_mode) {
        	init_time += last_frame_duration;
        	if (pause_mode == ONE_FRAME) {
        		pause_mode = PAUSE;
        	} else {
        		sf:sleep(sf::seconds(0.02));
        		return true;
        	}
        }

        ++frames_cnt;

        // mouse_pos.print();
        // printf(" <mouse\n");

		logic_tick();

		while (cur_time - init_time > current_physics_time + PHYSICS_ENGINE_TIME_STAMP) {
			physics_tick();
			current_physics_time += PHYSICS_ENGINE_TIME_STAMP;
		}
		// if (frames_cnt % 2) screen_tick();
		visual_render_tick();

		for (int i = 0; i < objects.size(); ++i) {
			objects[i]->render_approximation(visual.pmap);
		}

		screen_tick();

		return true;
	}

	void everlasting_loop() {
		while (tick());
	}

//  ===== get / set =====

	Vec2d get_mouse_pos() {
		return mouse_pos;
	}

	double get_cur_time() {
		return current_time;
	}

	bool second_has_passed() {
		return second_passed;
	}

//  =====================

	void destroy() {
		visual.destroy();
	}
};

#endif // GAME_ENGINE