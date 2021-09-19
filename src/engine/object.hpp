#ifndef OBJECT
#define OBJECT

#include "../visual/renderable.hpp"
#include "../physics/tickable.hpp"
#include "../physics/collidable.hpp"
#include "kctf_engine_prot.hpp"

#include <vector>

int MAX_OBJECT_ID = 0;

class Object : public Renderable, public Tickable, public Collidable
{
	std::vector<Object*> children;
public:
	KCTF_Engine *eng;
	int id;
	bool del_logic;

	Object():
	eng(nullptr),
	Renderable(MAX_OBJECT_ID++),
	Tickable(),
	id(MAX_OBJECT_ID++),
	del_logic(false)
	{}

	void set_engine(KCTF_Engine *eng_) {
		eng = eng_;
	}

	void obj_delete() {
		del_render    = true;
		del_tick      = true;
		del_collision = true;
		del_logic     = true;
	}

	virtual void logic_tick(const double time, const double dt) {};
};

#endif // OBJECT