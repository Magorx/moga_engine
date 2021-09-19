#include "moga_engine.h"


void MogaEngine::frame_init_tick() {
	prev_tick_time = current_time;
	current_time = (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - init_time) / 1000000000.0;
	// printf("%g\n", current_time);

    dt = prev_tick_time - current_time;

    if (!pause_mode) {
		fps_seconds_tick = false;
		double fps_seconds_elapsed = current_time - fps_second_start;
        if (fps_seconds_elapsed >= 1.0) {
        	fps_seconds_tick = true;
            // if (frames_cnt < CRITICAL_FPS_RATE) {
            //     pause_mode = 1;
            // }

            // window.setTitle("KCTF | FPS: " + std::to_string(frames_cnt));
            fps = frames_cnt;
            fps_second_start = current_time;
            frames_cnt = 0;
        }
    }
}

void MogaEngine::visual_render_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[visual_tick] %lg\n", current_time);
	#endif
	visual->tick();
}

void MogaEngine::physics_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[physics_tick] %lg\n", current_physics_time);
	#endif

	// physics.tick(current_physics_time, PHYSICS_ENGINE_TIME_STAMP, visual.pmap);
}

void MogaEngine::logic_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[logic_tick] %lg\n", current_time);
	#endif

	// int deleted_cnt = 0;
	// for (size_t i = 0; i < objects.size(); ++i) {
	// 	if (objects[i]->del_logic) {
	// 		++deleted_cnt;
	// 		continue;
	// 	}

	// 	objects[i]->logic_tick(current_time, dt);
	// }

	// if (deleted_cnt > objects.size() * OBJECTS_BUFFER_REFRESH_COEF && deleted_cnt) {
	// 	int next_alive = 0;
	// 	int i = 0;
	// 	int objects_cnt = objects.size();
	// 	for (; i < objects_cnt; ++i) {
	// 		if (objects[i]->del_logic) {
	// 			for (next_alive = std::max(i + 1, next_alive);
	// 				 next_alive < objects_cnt && objects[next_alive]->del_logic;
	// 				 ++next_alive); // find next alive object

	// 			if (next_alive < objects_cnt) {
	// 				std::swap(objects[i], objects[next_alive]);
	// 				++next_alive;
	// 			}
	// 		}
	// 	}
	// 	objects.resize(objects.size() - deleted_cnt);
	// }
}

void MogaEngine::handle_events() {}

MogaEngine::MogaEngine(const char  *window_name,
					   const size_t screen_width,
					   const size_t screen_height,
					   const size_t):
	// physics(),
	// objects(),

	current_time(0),
	prev_tick_time(0),
	physics_current_time(0),
	dt(0),

	init_time(0),
    fps_second_start(0),
    
    frames_cnt(0),
    fps(0),
    pause_mode(0),

	fps_seconds_tick(false),

    mouse_pos(0, 0),

	visual(new VisualEngine(window_name, screen_width, screen_height))
{
	init_time            = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	current_time         = init_time;
	physics_current_time = init_time;
	fps_second_start     = init_time;
}

bool MogaEngine::add_tickable(Tickable *object) {
	tickables.push_back(object);
	return true;
}

// bool MogaEngine::add_physics_tickable(Tickable *object) {
// 	return physics.add_tickable(object);
// }

// bool MogaEngine::add_physics_collidable(Collidable *object) {
// 	return physics.add_collidable(object);
// }

bool MogaEngine::add_renderable(Renderable *object) {
	return visual->add_renderable(object);
}

// bool MogaEngine::add_object(Object *object, bool is_collidable) {
// 	if (!object) {
// 		return false;
// 	}

// 	objects.push_back(object);
// 	add_physics_tickable(object);
// 	add_visual_renderable(object);

// 	object->update_approximation(current_time);

// 	if (is_collidable) {			
// 		add_physics_collidable(object);
// 	}

// 	object->set_engine(this);

// 	return true;
// }

void MogaEngine::tick(const double, const double) {
	frame_init_tick();
	handle_events();

	if (pause_mode) {
    	if (pause_mode == ONE_FRAME) {
    		pause_mode = PAUSE;
    	} else {
    		std::this_thread::sleep_for(std::chrono::milliseconds(50));
    		return;
    	}
    }

    ++frames_cnt;

    for (size_t i = 0; i < tickables.size(); ++i) {
    	tickables[i]->tick(dt, current_time);
    }
    logic_tick();

	while (current_time - physics_current_time > 0.02) {
		physics_tick();
		physics_current_time += 0.02;
	}

	visual_render_tick();

	return;
}

void MogaEngine::everlasting_loop() {
	while (visual->get_renderer()->get_window()->isOpen()) {
		tick();
	}
}

//  ===== get / set =====

Vec3d MogaEngine::get_mouse_pos() const {
	return mouse_pos;
}
