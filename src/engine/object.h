#ifndef OBJECT
#define OBJECT

#include "tickable.h"
#include "visual/renderer/renderable.h"
#include "physics/phys_tickable.h"
#include "physics/solid_body/solid_body.h"

#include <vector>

class Object : public Tickable
{
protected:
	Renderable *texture;
	SolidBody  *solid_body;

public:
	bool del_logic;

	Object(Renderable *texture, SolidBody  *solid_body):
	texture(texture),
	solid_body(solid_body),
	del_logic(false)
	{}

	void obj_delete() {
		del_logic = true;
		texture->del();
		solid_body->del();
	}

	virtual void tick(const double = 0, const double = 0) override {};

	inline Renderable *get_texture()    { return texture;    }
	inline SolidBody  *get_solid_body() { return solid_body; }
};

#endif // OBJECT