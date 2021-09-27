#ifndef MOGA_ENGINE_H
#define MOGA_ENGINE_H

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <thread>
#include <chrono>

#include "moga_engine_settings.h"
#include "../physics/physics_engine.h"
#include "../visual/visual_engine.h"

#include "tickable.h"
#include "object.h"

#include <vector>

class MogaEngine : Tickable
{
protected:
	std::vector<Tickable*> tickables;
	std::vector<Object*> objects;

	double current_time;
	double prev_tick_time;
	double physics_current_time;
	double dt;

	time_t init_time;
    double fps_second_start;
    
    int frames_cnt;
    int fps;
    int pause_mode;

	bool fps_seconds_tick;

    Vec3d mouse_pos;
//  =====================

public:
	VisualEngine  *visual;
	PhysicsEngine *physics;

	MogaEngine(const char  *window_name,
				const size_t screen_width,
				const size_t screen_height,
				const size_t pixel_size);
	
	virtual ~MogaEngine();

	bool add_tickable(Tickable *object);
	bool add_physics_tickable(PhysTickable *object);
	bool add_solid_body(SolidBody *object);
	bool add_renderable(Renderable *object);
	bool add_object(Object *object, bool is_collidable = true);

	virtual void frame_init_tick();
	virtual void visual_render_tick();
	virtual void physics_tick();
	virtual void tickable_tick();
	virtual void logic_tick();

	virtual void handle_events();

	virtual void everlasting_loop();

	void tick(const double dt = 0, const double current_time = 0) override;

	void clear_deleted_tickables();
	void clear_deleted_objects();

//  ===== get / set =====

	Vec3d  get_mouse_pos() const;

	inline double get_cur_time() const {
		return current_time;
	}
	
	inline bool fps_seconds_ticked() const  {
		return fps_seconds_tick;
	};

//  =====================
};

#endif // MOGA_ENGINE_H