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
	main_view->e_render_call.emit({visual->get_renderer()}, true);
	visual->get_renderer()->display();
}

void MogaEngine::physics_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[physics_tick] %lg\n", current_physics_time);
	#endif

	physics->tick(PHYSICS_TIME_STEP * physics_time_multiplier, physics_current_time);
}

void MogaEngine::tickable_tick() {
	int deleted_cnt = 0;
	for (size_t i = 0; i < tickables.size(); ++i) {
		if (tickables[i]->del_tickable) {
			++deleted_cnt;
			continue;
		}

    	tickables[i]->tick(dt, current_time);
    }	

	if (deleted_cnt > tickables.size() * 0.2) {
		clear_deleted_tickables();
	}

	deleted_cnt = 0;
	for (size_t i = 0; i < objects.size(); ++i) {
		if (objects[i]->del_tickable) {
			++deleted_cnt;
			continue;
		}

    	objects[i]->tick(dt, current_time);
    }	

	if (deleted_cnt > objects.size() * 0.2) {
		clear_deleted_objects();
	}
}

void MogaEngine::clear_deleted_tickables() {
	size_t next_alive = 0;
    size_t tickables_cnt = tickables.size();
    size_t i = 0;

    for (; i < tickables_cnt; ++i) {
        if (!tickables[i] || tickables[i]->del_tickable) {
            for (next_alive = std::max(i + 1, next_alive);
                next_alive < tickables_cnt && tickables[next_alive] && tickables[next_alive]->del_tickable;
                ++next_alive); // find next alive object

            if (next_alive < tickables_cnt) {
                if (tickables[i] && !tickables[i]->tickable_nonfree) {
                    delete tickables[i];
                }

                tickables[i] = tickables[next_alive];
                tickables[next_alive] = nullptr;
                ++next_alive;
            } else {
                break;
            }
        }
    }

    tickables.resize(i);
}

void MogaEngine::clear_deleted_objects() {
	size_t next_alive = 0;
    size_t objects_cnt = objects.size();
    size_t i = 0;

    for (; i < objects_cnt; ++i) {
        if (!objects[i] || objects[i]->del_tickable) {
            for (next_alive = std::max(i + 1, next_alive);
                next_alive < objects_cnt && objects[next_alive] && objects[next_alive]->del_tickable;
                ++next_alive); // find next alive object

            if (next_alive < objects_cnt) {
                if (objects[i]) {
                    delete objects[i];
                }

                objects[i] = objects[next_alive];
                objects[next_alive] = nullptr;
                ++next_alive;
            } else {
                break;
            }
        }
    }

    objects.resize(i);
}

void MogaEngine::logic_tick() {
	#ifdef PRINT_PHASE_TICK
	printf("[logic_tick] %lg\n", current_time);
	#endif
}

void MogaEngine::on_mouse_click(Vec2d click) {
	main_view->e_mouse_press.emit({click});
}

void MogaEngine::on_mouse_hover(Vec2d hover) {
	main_view->e_mouse_move.emit({mouse_pos, hover});
}

void MogaEngine::on_mouse_release(Vec2d click) {
		main_view->e_mouse_release.emit({click});
}

void MogaEngine::handle_events(sf::RenderWindow &window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseButtonPressed) {
			on_mouse_click({(double) event.mouseButton.x, (double) event.mouseButton.y});
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			on_mouse_release({(double) event.mouseButton.x, (double) event.mouseButton.y});
		}

		if (event.type == sf::Event::MouseMoved) {
			Vec2d hover = {(double) event.mouseMove.x, (double) event.mouseMove.y};
			// if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			// 	main_view->e_mouse_drag.emit({mouse_pos, hover, Event::MouseDrag::left});
			// } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			// 	main_view->e_mouse_drag.emit({mouse_pos, hover, Event::MouseDrag::right});
			// } else {
				on_mouse_hover(hover);
			// }
			mouse_pos = {(double) event.mouseMove.x, (double) event.mouseMove.y};
		}
	}
}

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
	physics_time_multiplier(1),

	init_time(0),
    fps_second_start(0),
    
    frames_cnt(0),
    fps(0),
    pause_mode(0),

	fps_seconds_tick(false),

    mouse_pos(0, 0),

	visual(new VisualEngine(window_name, screen_width, screen_height)),
	physics(new PhysicsEngine()),
	main_view(new AbstractView(ViewBody{{0, 0}, {(double) screen_width, (double) screen_height}}))
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

	add_renderable(object->get_texture());

	if (is_collidable) {
		add_solid_body(object->get_solid_body());
	}

	return true;
}

bool MogaEngine::add_view(AbstractView *view) {
	main_view->add_subview(view);

	view->tickable_nonfree = true;
	add_tickable(view);

	// add_renderable(view);

	return true;
}

void MogaEngine::tick(const double, const double) {
	frame_init_tick();
	handle_events(*visual->get_renderer()->get_window());

	if (pause_mode) {
    	if (pause_mode == ONE_FRAME) {
    		pause_mode = PAUSE;
    	} else {
    		std::this_thread::sleep_for(std::chrono::milliseconds(50));
    		return;
    	}
    }

    ++frames_cnt;

	tickable_tick();

	while (current_time - physics_current_time > PHYSICS_TIME_STEP) {
		physics_tick();
		physics_current_time += PHYSICS_TIME_STEP;
	}

	logic_tick();

	visual_render_tick();

	return;
}

void MogaEngine::everlasting_loop() {
	sf::RenderWindow *window = visual->get_renderer()->get_window();

	while (window->isOpen()) {
		tick();
	}
}

//  ===== get / set =====

Vec3d MogaEngine::get_mouse_pos() const {
	return mouse_pos;
}

MogaEngine::~MogaEngine() {
	printf("hoya\n");
	for (auto x : tickables) {
		if (!x->tickable_nonfree) delete x;
	}

	for (auto x : objects) {
		delete x;
	}

	delete visual;
	delete physics;
}
