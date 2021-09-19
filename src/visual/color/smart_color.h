#ifndef SMART_COLOR
#define SMART_COLOR

#include "color/color.h"
#include "engine/tickable.h"

#include <vector>

class SmartColor : Tickable {
	Color color;

public:
	SmartColor();
	SmartColor(const Color color_);

	virtual const Color rgb();

	void set_rgb(const Color color_);
	void set_rgb(const double r, const double g, const double b);
};

#endif // SMART_COLOR