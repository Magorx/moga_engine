#ifndef POINT_JOINT
#define POINT_JOINT

#include "tickable.hpp"
#include "physic_point.hpp"

class PointJoint : public Tickable
{
public:
	PhysicPoint &p1;
	PhysicPoint &p2;

	double len;
	double rigidity;

	bool is_static;

	PointJoint(	PhysicPoint &p1_, 
				PhysicPoint &p2_,
				double rigidity_ = 0,
				double len_ = -1,
				double is_static_ = false):
	p1(p1_),
	p2(p2_),
	rigidity(rigidity_),
	len(len_),
	is_static(is_static_)
	{
		if (len < 0) {
			len = (p1.pos - p2.pos).len();
		}
	}

	void tick(const double dt) {
		Vec2d axis = p2.pos - p1.pos;
		double dl = len - axis.len();
		axis.normalize();

		if (len && axis.len() < PHYSIC_EPS) {
			Vec2d rand_dir = random_uni_vector();
			p2.pos += rand_dir * len / 10;
			return;
		}

		if (is_static) {
			if (p1.is_static) {
				if (p2.is_static) {
					printf("[ERR] two static points have static joint and moved\n");
				} else {
					p2.pos += axis * dl;
				}
			} else if (p2.is_static) {
				p1.pos -= axis * dl;
			} else {
				p1.pos -= axis * dl / 2;
				p2.pos += axis * dl / 2;
			}
			return;
		}

		double imp = dl * rigidity;
		p1.apply_impulse(axis * -imp * dt);
		p2.apply_impulse(axis * +imp * dt);
	}
};

#endif // POINT_JOINT