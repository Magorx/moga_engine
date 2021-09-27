#ifndef TICKABLE
#define TICKABLE

class Tickable
{
public:
	bool del_tickable;

	Tickable():
	del_tickable(false)
	{}

	virtual ~Tickable() {
		// del_tickable = true;
	}

	virtual void tick(const double = 0, const double = 0) {};

	inline void del() { del_tickable = true; }
};

#endif // TICKABLE
