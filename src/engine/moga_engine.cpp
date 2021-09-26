#include "moga_engine.h"


void MogaEngine::frame_init_tick() {
	prev_tick_time = current_time;
	current_time = (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) / 1000000000.0 - init_time;
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

	physics->tick(PHYSICS_TIME_STEP, physics_current_time);
}

void MogaEngine::logic_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[logic_tick] %lg\n", current_time);
	#endif
}

void MogaEngine::handle_events() {}

MogaEngine::MogaEngine(const char  *window_name,
					   const size_t screen_width,
					   const size_t screen_height,
					   const size_t):
	tickables(),
	objects(),

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

	visual(new VisualEngine(window_name, screen_width, screen_height)),
	physics(new PhysicsEngine())
{
	init_time            = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000000000.0;
	current_time         = 0;
	physics_current_time = 0;
	fps_second_start     = 0;

	// printf("!!!!!!!!!! %g = %g\n", physics_current_time, init_time);
}

bool MogaEngine::add_tickable(Tickable *object) {
	tickables.push_back(object);
	return true;
}

bool MogaEngine::add_physics_tickable(PhysTickable *object) {
	return physics->add_phys_tickable(object);
}

bool MogaEngine::add_solid_body(SolidBody *object) {
	return physics->add_solid(object);
}

bool MogaEngine::add_renderable(Renderable *object) {
	return visual->add_renderable(object);
}

bool MogaEngine::add_object(Object *object, bool is_collidable) {
	if (!object) {
		return false;
	}

	objects.push_back(object);

	tickables.push_back(object);
	add_renderable(object->get_texture());

	if (is_collidable) {
		add_solid_body(object->get_solid_body());
	}

	return true;
}

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

	while (current_time - physics_current_time > PHYSICS_TIME_STEP) {
		physics_tick();
		physics_current_time += PHYSICS_TIME_STEP;
	}

	logic_tick();

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
