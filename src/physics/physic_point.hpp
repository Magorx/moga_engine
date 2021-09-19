#ifndef PHYSIC_POINT
#define PHYSIC_POINT

#include "tickable.hpp"
#include "../general/cpp/vec2d.hpp"

class PhysicPoint : public Tickable
{
public:
	double mass;

	Vec2d pos;
	Vec2d velocity;
	double angle;
	double angle_speed;
	bool is_static;

	PhysicPoint(double mass_,
				bool   is_static_= false):
	mass(mass_),
	pos(),
	velocity(),
	angle(0),
	angle_speed(0),
	is_static(is_static_)
	{}

	PhysicPoint(double mass_     = 0, 
				Vec2d  pos_      = {0, 0},
				bool   is_static_= false,
				Vec2d  velocity_ = {0, 0},
				double angle_       = 0,
				double angle_speed_ = 0):
	mass(mass_),
	pos(pos_),
	velocity(velocity_),
	angle(angle_),
	angle_speed(angle_speed_),
	is_static(is_static_)
	{}

	void apply_impulse(const Vec2d impulse) {
		if (is_static) {
			return;
		}

		velocity += impulse / mass;
	}

	void apply_impulse(const Vec2d point, const Vec2d impulse, const double I = 1, double coef = 1) {
		if (is_static) {
			return;
		}

		apply_impulse(impulse * coef);

		Vec2d p_o = pos - point;
		double proection = impulse.normal().dot(p_o);
		Vec2d proected  = impulse.normal() * proection + point;
		double arm = (proected - pos).len();

		Vec2d o_p = point - pos;
		double dp = o_p.cross(impulse);

		angle_speed += dp / I;

		// angle_speed += vec2d_sign(impulse.cross(p_o)) * impulse.len() * arm / mass / 1000;
	}

	void tick(const double dt) {
		if (velocity.len() < 0.01) {
			velocity.nullify();
		}
		pos += velocity * dt;

		if (fabs(angle_speed) < 0.0001) {
			angle_speed = 0;
		}
		angle += angle_speed;
	}
};

#endif // PHYSIC_POINT