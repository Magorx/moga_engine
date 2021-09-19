#ifndef GAME_ENGINE_PROT
#define GAME_ENGINE_PROT

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>

#include "kctf_engine_settings.hpp"
#include "../physics/physics_engine.hpp"
#include "../visual/visual_engine.hpp"

#include <vector>

class Object;

class KCTF_Engine
{
protected:
	std::vector<Object*> objects;
	std::vector<Tickable*> general_tickables;

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

public:
	VisualEngine  visual;
	PhysicsEngine physics;

	KCTF_Engine(const size_t screen_width,
				const size_t screen_height,
				const size_t pixel_size);

	bool add_tickable(Tickable *object);
	bool add_physics_tickable(Tickable *object);
	bool add_physics_collidable(Collidable *object);
	bool add_visual_renderable(Renderable *object);
	bool add_object(Object *object, bool is_collidable = false);

	virtual void frame_init_tick();
	virtual void visual_render_tick();
	virtual void physics_tick();
	virtual void logic_tick();
	virtual void screen_tick();

	virtual void handle_events();

	virtual bool tick();

	virtual void everlasting_loop();

//  ===== get / set =====

	Vec2d get_mouse_pos();
	double get_cur_time();
	bool second_has_passed();

//  =====================

	void destroy();
};

#endif // GAME_ENGINE_PROT