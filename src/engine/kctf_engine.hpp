#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "kctf_engine_prot.hpp"
#include "objects_collection.hpp"

SmartColor *SOME_COLOR = nullptr;

void KCTF_Engine::frame_init_tick() {
	cur_time = clock.getElapsedTime().asSeconds();
    last_frame_duration = cur_time - prev_frame_time;
    prev_frame_time = cur_time;

    absolute_mouse_pos = sf::Mouse::getPosition(window);
    mouse_pos = Vec2d(absolute_mouse_pos.x / visual.pixel_size, absolute_mouse_pos.y / visual.pixel_size);

    current_time  = cur_time - init_time;
    dt = last_frame_duration;

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

void KCTF_Engine::visual_render_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[visual_tick] %lg\n", current_time);
	#endif
	visual.render(current_time);
    // visual.flush_to_texture(scaling_texture);
    // window.draw(scaling_sprite);
    // window.display();
}

void KCTF_Engine::physics_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[physics_tick] %lg\n", current_physics_time);
	#endif

	physics.tick(current_physics_time, PHYSICS_ENGINE_TIME_STAMP, visual.pmap);
}

void KCTF_Engine::logic_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[logic_tick] %lg\n", current_time);
	#endif

	int deleted_cnt = 0;
	for (size_t i = 0; i < objects.size(); ++i) {
		if (objects[i]->del_logic) {
			++deleted_cnt;
			continue;
		}

		objects[i]->logic_tick(current_time, dt);
	}

	if (deleted_cnt > objects.size() * OBJECTS_BUFFER_REFRESH_COEF && deleted_cnt) {
		int next_alive = 0;
		int i = 0;
		int objects_cnt = objects.size();
		for (; i < objects_cnt; ++i) {
			if (objects[i]->del_logic) {
				for (next_alive = std::max(i + 1, next_alive);
					 next_alive < objects_cnt && objects[next_alive]->del_logic;
					 ++next_alive); // find next alive object

				if (next_alive < objects_cnt) {
					std::swap(objects[i], objects[next_alive]);
					++next_alive;
				}
			}
		}
		objects.resize(objects.size() - deleted_cnt);
	}
}

void KCTF_Engine::screen_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[screen_tick] %lg\n", current_time);
	#endif
	visual.flush_to_texture(scaling_texture);
    window.draw(scaling_sprite);
    window.display();
}

void KCTF_Engine::handle_events() {}

KCTF_Engine::KCTF_Engine(const size_t screen_width,
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

bool KCTF_Engine::add_tickable(Tickable *object) {
	general_tickables.push_back(object);
	return true;
}

bool KCTF_Engine::add_physics_tickable(Tickable *object) {
	return physics.add_tickable(object);
}

bool KCTF_Engine::add_physics_collidable(Collidable *object) {
	return physics.add_collidable(object);
}

bool KCTF_Engine::add_visual_renderable(Renderable *object) {
	return visual.add_renderable(object);
}

bool KCTF_Engine::add_object(Object *object, bool is_collidable) {
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

	object->set_engine(this);

	return true;
}

bool KCTF_Engine::tick() {
	if (!window.isOpen()) {
		return false;
	}

	frame_init_tick();
	handle_events();

	if (pause_mode) {
    	if (pause_mode == ONE_FRAME) {
    		pause_mode = PAUSE;
    	} else {
    		init_time += last_frame_duration;
    		sf:sleep(sf::seconds(0.02));
    		return true;
    	}
    }

    ++frames_cnt;

    for (int i = 0; i < general_tickables.size(); ++i) {
    	general_tickables[i]->tick(dt);
    }
    logic_tick();

	while (cur_time - init_time > current_physics_time + PHYSICS_ENGINE_TIME_STAMP) {
		physics_tick();
		current_physics_time += PHYSICS_ENGINE_TIME_STAMP;
	}

	visual_render_tick();
	screen_tick();

	return true;
}

void KCTF_Engine::everlasting_loop() {
	while (tick());
}

//  ===== get / set =====

Vec2d KCTF_Engine::get_mouse_pos() {
	return mouse_pos;
}

double KCTF_Engine::get_cur_time() {
	return current_time;
}

bool KCTF_Engine::second_has_passed() {
	return second_passed;
}

//  =====================

void KCTF_Engine::destroy() {
	visual.destroy();
}

#endif // GAME_ENGINE