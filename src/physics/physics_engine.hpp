#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#include "physics_settings.hpp"
#include "tickable.hpp"

#include "physic_objects_collection.hpp"
#include <vector>
#include <set>

struct Inters {
	Collidable &obj1;
	Collidable &obj2;
	PhysicPoint &point;
	Vec2d impulse;

	Inters(Collidable &obj1_, Collidable &obj2_, PhysicPoint &point_, Vec2d impulse_):
	obj1(obj1_),
	obj2(obj2_),
	point(point_),
	impulse(impulse_)
	{}
};

int COLLISION_COUNTER = 0;

class PhysicsEngine
{
public:
	std::vector<Tickable*  > tickables;
	std::vector<Collidable*> collidables;
	std::vector<Inters> fixes;

	Vec2d gravity;

	PhysicsEngine(Vec2d gravity_ = Vec2d(0, 1)):
	gravity(gravity_.normal() * GRAVITY_COEF)
	{}

	bool add_tickable(Tickable *object) {
		tickables.push_back(object);
		return true;
	}

	bool add_collidable(Collidable *object) {
		collidables.push_back(object);
		return true;
	}

	void chain_joint_points(std::vector<PhysicPoint> &points, double rigidity = 0, double len = -1, bool static_joint = true) {
		for (int i = 0; i < points.size(); ++i) {
			add_tickable(new PointJoint(points[i], points[i + 1], rigidity, len, static_joint));
		}
	}

	void global_affects(const double time, const double dt, PixelMap &pmap) {
		//printf("===grav===\n");
		for (int i = 0; i < collidables.size(); ++i) {
			Collidable &obj = *collidables[i];
			PhysicPoint &mc = obj.mass_center;

			mc.apply_impulse(gravity * mc.mass * dt);

			mc.velocity    *= ENERGY_LOSS_COEF;
			mc.angle_speed *= ENERGY_LOSS_COEF;

			if (mc.velocity.len() > MAX_VELOCITY_MODULE) {
				if (mc.velocity.len() > MAX_VELOCITY_MODULE) {
					mc.velocity.normalize();
					mc.velocity *= MAX_VELOCITY_MODULE * 1.5;
				}
				mc.velocity *= 0.25;
			}

			if (fabs(mc.angle_speed) > MAX_ANGLE_SPEED) {
				if (mc.angle_speed > MAX_ANGLE_SPEED * 2) {
					mc.angle_speed = MAX_ANGLE_SPEED * 1.5 * vec2d_sign(mc.angle_speed);
				}
				mc.angle_speed *= 0.25;
			}

			//obj.mass_center.apply_impulse(obj.mass_center.pos + Vec2d(10, 10), Vec2d(0, -10) * dt, obj.I);
			//pmap.draw_square_circle({obj.mass_center.pos, {0, 255, 0}, 0}, 2);
			//printf("v = %lg\n", obj.mass_center.velocity.len());
		}
	}

	void tick(const double time, const double dt, PixelMap &pmap) {
		// printf("==============================================================================\n");
		// for (int i = 0; i < fixes.size(); ++i) {
		// 	Inters &x = fixes[i];
		// 	x.obj2.mass_center.apply_impulse(x.point.pos, x.impulse * -dt * 0.75, x.obj2.I, 0);
		// 	x.obj1.mass_center.apply_impulse(x.point.pos, x.impulse * +dt * 0.75, x.obj1.I, 0);
		// 	// x.obj2.mass_center.tick(dt);
		// 	// x.obj1.mass_center.tick(dt);
		// }
		// fixes.clear();

		// global_affects(time, dt, pmap);

		int deleted_cnt = 0;
		for (int i = 0; i < tickables.size(); ++i) {
			if (tickables[i]->del_tick) {
				++deleted_cnt;
				continue;
			}

			tickables[i]->tick(dt);
		}

		if (deleted_cnt > tickables.size() * TICKABLE_BUFFER_REFRESH_COEF && deleted_cnt) {
			int next_alive = 0;
			int i = 0;
			int tickables_cnt = tickables.size();
			for (; i < tickables_cnt; ++i) {
				if (tickables[i]->del_tick) {
					for (next_alive = std::max(i + 1, next_alive); 
						 next_alive < tickables_cnt && tickables[next_alive]->del_tick;
						 ++next_alive); // find next alive object

					if (next_alive < tickables_cnt) {
						std::swap(tickables[i], tickables[next_alive]);
						++next_alive;
					}
				}
			}
			tickables.resize(tickables.size() - deleted_cnt);
		}

		return; //============================================================================= remove this return

		for (int i = 0; i < collidables.size(); ++i) {
			collidables[i]->update_approximation(time);
			collidables[i]->render_approximation(pmap);
		}

		for (int q = 0; q < 3; ++q) {
		std::set<std::pair<int, int>> collided;
		for (int i = 0; i < collidables.size(); ++i) {
			for (int j = 0; j < collidables.size(); ++j) {
				std::pair<int, int> label(std::min(i, j), std::max(i, j));
				if (i == j) continue;
				collided.insert(label);

				Collidable &obj1 = *collidables[i];
				Collidable &obj2 = *collidables[j];
				double bias = 0.3;

				fixes.clear();
				for (int i = 0; i < obj2.approximation.size(); ++i) {
					PhysicPoint &p = obj2.approximation[i];

					if (obj1.is_point_inside(p.pos)) {
						PhysicPoint *point = &p; // obj1.collides_with(obj2);
						COLLISION_COUNTER++;
						#ifdef SHOW_UHHH_COLLISION
						printf("uhhh %d\n", COLLISION_COUNTER);
						#endif

						// ===== solve the collision =====
						// FIND THE BEST COLLISION POINT

						int edge_index = obj1.find_nearest_edge_index(point->pos, bias);
						if (edge_index < 0) {
							collided.erase(label);
							continue;
						}

						PhysicPoint &np1 = obj1.approximation[edge_index    ];
						PhysicPoint &np2 = obj1.approximation[edge_index + 1];

						Vec2d np12 = (np2.pos - np1.pos).normal();
						Vec2d hit = np1.pos + np12 * np12.dot(point->pos - np1.pos);

						pmap.draw_square_circle({hit,        {0, 0, 255}, 0}, 1);
						pmap.draw_square_circle({point->pos, {0, 255, 0}, 0}, 1);

						pmap.draw_square_circle({np1.pos, {255, 255, 0}, 0}, 1);
						pmap.draw_square_circle({np2.pos, {0, 255, 255}, 0}, 1);

						double magic_coef = 0;
						Vec2d shift = (hit - point->pos) * (1 + magic_coef);
						hit += shift * magic_coef;

						// if (obj2.mass_center.velocity.len() < 0.1) {
						// 	obj2.collision_shift += shift;
						// 	obj2.mass_center.velocity.nullify();
						// 	obj2.mass_center.angle_speed = 0;
						// 	continue;
						// }

						Vec2d N = shift.normal();
						Vec2d NN = N;

						// N *= -1;
						// N = N * N.dot(obj2.mass_center.velocity);
						// obj2.mass_center.velocity -= N;

						N = NN;
						N = rot_clockwise(N, Pi/2);
						N = N.orient(obj2.mass_center.velocity);
						N = N * N.dot(obj2.mass_center.velocity);
						obj2.mass_center.apply_impulse(N * -0.5 * dt);

						// printf("%lg NN\n", NN.len());
						N = NN;

						PhysicPoint &mc = obj2.mass_center;
						Vec2d  center = obj2.mass_center.pos;
						Vec2d  c_point = point->pos - center;
						double depth = shift.len();
						double base = 2;
						Vec2d  speed = obj2.mass_center.velocity;
						double spd_proec = (N * -1).dot(obj2.mass_center.velocity);

						Vec2d next_center = center + speed * dt;
						Vec2d next_point  = point->pos  + speed * dt;
						double ang = (next_point - next_center).angle(hit - next_center);
						double req_ang_spd = ang / dt;

						double l = 0;
						double r = 10000 * obj2.mass_center.mass;

						if (depth > bias) {
							while (fabs(l - r) > 0.1) {
								double m = (l + r) / 2;
								Vec2d new_point = obj2.get_point_pos_prediction(point->pos, N * m, dt);
								double norm_shift = (new_point - point->pos).dot(N);
								double new_depth = depth - norm_shift;

								if (new_depth > 0) {
									l = m;
								} else {
									r = m;
								}
							}
						} else {
							Vec2d new_point = obj2.get_point_pos_prediction(point->pos, N * 0, dt);
							double norm_shift = (new_point - point->pos).dot(N);
							double new_depth = depth - norm_shift;
							if (new_depth < bias) {
								// printf("ok, going away\n");
								l = 0;
							} else {
								// printf("bias works\n");
								while (fabs(l - r) > 0.01) {
									double m = (l + r) / 2;
									Vec2d new_point = obj2.get_point_pos_prediction(point->pos, N * m, dt);
									double norm_shift = (new_point - point->pos).dot(N);
									double new_depth = depth - norm_shift;

									if (new_depth < bias / 2) {
										r = m;
									} else {
										if (new_depth > bias / 2) {
											l = m;
										} else {
											break;
										}
									}
								}
							}
						}

						// printf("vel = %lg angspd = %lg | depth = %lg | imp = %lg -> %lg \n", speed.len(), obj2.mass_center.angle_speed, depth, l, l * dt);
						// l = 1000;
						N *= l;

						// if (depth < 5) {
						// 	printf("spd %lg\n", spd_proec);
						// 	//N += (N * spd_proec * 10).orient(N) * obj2.mass_center.mass;
						// 	N -= gravity * obj2.mass_center.mass;
						// } else {
						// 	N = (N * pow(depth, 2)).orient(N) * obj2.mass_center.mass;
						// }

						obj2.mass_center.apply_impulse(point->pos, N * +dt, obj2.I);
						obj1.mass_center.apply_impulse(point->pos, N * -dt, obj1.I);

						// ===
						
						// double angle = (hit - obj2.mass_center.pos).angle(point->pos - obj2.mass_center.pos);
						// obj2.collision_angle += angle;
						// obj1.collision_angle += angle;

						// obj2.collision_shift += shift;
						// obj1.collision_shift -= shift;
					}

				}

				// if (fixes.size()) {
				// 	double coef = 1 / fixes.size();
				// 	for (int i = 0; i < fixes.size(); ++i) {
				// 		Inters &x = fixes[i];
				// 		x.obj2.mass_center.apply_impulse(x.point.pos, x.impulse * dt, x.obj2.I, 0);
				// 		x.obj1.mass_center.apply_impulse(x.point.pos, x.impulse * dt, x.obj1.I, 0);
				// 		// x.obj2.mass_center.tick(dt);
				// 		// x.obj1.mass_center.tick(dt);
				// 	}
				// }
			}
		}
		}
	}
};

#endif // PHYSICS_ENGINE