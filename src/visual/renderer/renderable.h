#ifndef RENDERABLE
#define RENDERABLE

#include "renderer.h"

class Renderable {
public:
	int layer;
	bool del_render;

	inline Renderable(int layer_ = 0):
	layer(layer_),
	del_render(false)
	{}

	inline void set_layer(const int layer_) {
		layer = layer_;
	}

	virtual void render(Renderer *) {};
};

#endif // RENDERABLE