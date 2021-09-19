#ifndef TICKABLE
#define TICKABLE

class Tickable
{
public:
	bool del_tick;

	Tickable():
	del_tick(false)
	{}

	virtual void tick(const double dt) = 0;
};

#endif // TICKABLE