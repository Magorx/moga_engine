#ifndef TICKABLE
#define TICKABLE

class Tickable
{
public:
	bool deleted;

	Tickable():
	deleted(false)
	{}

	virtual void tick(const double = 0, const double = 0) {};
};

#endif // TICKABLE
