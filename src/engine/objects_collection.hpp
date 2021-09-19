#ifndef OBJECTS_COLLECTION
#define OBJECTS_COLLECTION

#include "object.hpp"
#include "../visual/plines_collection.hpp"
#include "../physics/physic_objects_collection.hpp"

class obj_Ball : public Object
{
public:
	pline_Circle v_circle;
	PhysicPoint  p_point;

	obj_Ball(SmartColor *color,
			 double radius,
			 double mass,
			 int fill_mode    = FILL,
			 SmartColor *fill_color = nullptr,
			 int static_mode = NOT_STATIC):
	v_circle (color, 0, 2 * Pi, radius, fill_mode, fill_color),
	p_point  (mass, static_mode)
	{}

	obj_Ball(Vec2d coord, 
			 SmartColor *color,
			 double radius,
			 double mass,
			 int  fill_mode   = FILL,
			 SmartColor *fill_color = nullptr,
			 int static_mode = NOT_STATIC):
	v_circle (coord, color, 0, 2 * Pi, radius, fill_mode, fill_color),
	p_point  (mass, coord, static_mode)
	{}

	void tick(double dt) {
		//p_point.apply_impulse(Vec2d(0, 1) * p_point.mass * 9.8 * dt * PHYSIC_SCALING_COEF);
		p_point.tick(dt);
	}

	void render(PixelMap &pmap, const double current_time, const Vec2d shift = {0, 0}, const double angle_shift = 0) {
		v_circle.o = p_point.pos;
		v_circle.render(pmap, current_time, shift, angle_shift);
	}

	virtual void update_approximation(const double time) {
		std::vector<Vec2d> appr_points = v_circle.get_approximation(time);
		approximation.resize(appr_points.size() + 1);

		double point_mass = p_point.mass / (appr_points.size() + 1);
		#ifdef SHOW_APPROXIMATION_NUMBERS
		printf("brocken into %Lu pieces id %d\n", appr_points.size(), id);
		#endif
		for (int i = 0; i < appr_points.size(); ++i) {
			approximation[i] = PhysicPoint(point_mass, appr_points[i], p_point.is_static);
			//printf("%lg %lg\n", appr_points[i].x, appr_points[i].y);
		}
		approximation[appr_points.size()] = PhysicPoint(point_mass, appr_points[0], p_point.is_static);
	};
};

class obj_Spring : public Object
{
public:
	pline_SqueezeSinSeg v_sinseg_1;
	pline_SqueezeSinSeg v_sinseg_2;
	
	PointJoint p_joint;

	obj_Spring( PhysicPoint &p1,
				PhysicPoint &p2,
				double rigidity,
				double len = -1,
				size_t chain_cnt = 5,
				SmartColor *color_1 = nullptr,
				SmartColor *color_2 = nullptr,
				double is_static = false,
				double ampl_1 = 4,
				double ampl_2 = 4,
				double squz_1 = 1,
				double squz_2 = 1,
				double freq_1 = 0,
				double freq_2 = 0,
				double ofst_1 = 0,
				double ofst_2 = Pi):
	v_sinseg_1(color_1, 0, chain_cnt * Pi, ampl_1, freq_1, squz_1, ofst_1),
	v_sinseg_2(color_2, 0, chain_cnt * Pi, ampl_2, freq_2, squz_2, ofst_2),
	p_joint(p1, p2, rigidity, len, is_static)
	{}

	void render(PixelMap &pmap, const double current_time, const Vec2d shift = {0, 0}, const double angle_shift = 0) {
		v_sinseg_1.set_from_to(p_joint.p1.pos, p_joint.p2.pos);
		v_sinseg_2.set_from_to(p_joint.p1.pos, p_joint.p2.pos);
		v_sinseg_1.render(pmap, current_time, shift, angle_shift);
		v_sinseg_2.render(pmap, current_time, shift, angle_shift);
	}

	void tick(const double dt) {
		p_joint.tick(dt);
	}
};

class obj_Polygon : public Object, public Figure
{
public:
	double mass;
	int static_mode;

	obj_Polygon(double mass_,
				Vec2d o_, 
				ParametricLine2d *line_left_,
				ParametricLine2d *line_right_,
				ParametricLine2d *line_up_,
				ParametricLine2d *line_down_,
				SmartColor *color_ = nullptr,
				bool to_unify_colors_ = true,
				int fill_mode_  = FILL,
				int static_mode_ = NOT_STATIC):
	Figure(o_, line_left_, line_right_, line_up_, line_down_, color_, to_unify_colors_, fill_mode_),
	mass(mass_),
	static_mode(static_mode_)
	{}

	void render(PixelMap &pmap, const double current_time, const Vec2d shift = {0, 0}, const double angle_shift = 0) {
		Figure::render(pmap, current_time, shift, angle_shift);
	}

	void tick(const double dt) {
		Vec2d prev_pos = mass_center.pos;
		double prev_angle = mass_center.angle;

		mass_center.tick(dt);

		Vec2d pos_shift = mass_center.pos - prev_pos;
		double angle_shift = mass_center.angle - prev_angle;

		collision_shift += pos_shift;
		collision_angle += angle_shift;

		if (!(static_mode & STATIC_ANGLE)) {
			//collision_shift -= appr_center.pos;
			//appr_center.pos = hit_solution_point.pos + rot_clockwise(appr_center.pos - hit_solution_point.pos, collision_angle);
			//collision_shift += appr_center.pos;

			if (fabs(collision_angle) < 0.00) {
				collision_angle = 0;
			}

			o = rot_counterclockwise(o - mass_center.pos, collision_angle) + mass_center.pos;

			angle += collision_angle;
			collision_angle = 0;
		} else {
			mass_center.angle_speed = 0;
		}

		if (!(static_mode & STATIC_CENTER)) {
			if (fabs(collision_shift.len()) < 0.0) {
				collision_shift.nullify();
			}

			o += collision_shift;
			collision_shift.nullify();
		} else {
			mass_center.velocity.nullify();
		}
	}

	virtual void update_approximation(const double time) {
		std::vector<Vec2d> appr_points;
		if (!(static_mode & STATIC_CENTER)) {
			appr_points = get_approximation(time, collision_shift);
		} else {
			appr_points = get_approximation(time);
		}

		approximation.resize(appr_points.size());

		double point_mass = mass / (appr_points.size() - 1);
		Vec2d mean_point;
		for (int i = 0; i < appr_points.size() - 1; ++i) {
			approximation[i] = PhysicPoint(point_mass, appr_points[i], static_mode);
			mean_point += appr_points[i];
			//printf("%lg %lg\n", appr_points[i].x, appr_points[i].y);
		}
		mean_point /= (appr_points.size() - 1);
		approximation[appr_points.size() - 1] = PhysicPoint(0, appr_points[0], static_mode);
		
		mass_center.mass = mass;
		mass_center.pos = mean_point;

		I = 0;
		for (int i = 0; i < approximation.size(); ++i) {
			double l = (mean_point - approximation[i].pos).len();
			I += approximation[i].mass * l * l;
		}
	};
};

class obj_Box : public obj_Polygon
{
public:
	obj_Box(double mass,
			Vec2d size,
			SmartColor *color_    = nullptr,
			bool to_unify_colors_ = true,
			int fill_mode_        = FILL,
			Vec2d  angle_pos  = Vec2d(0, 0),
			int static_mode_ = NOT_STATIC):
	obj_Polygon(mass,
				angle_pos,
				new pline_Segment({-size.x/2, -size.y/2}, {-size.x/2, +size.y/2}, color_, 0, size.y),
				new pline_Segment({+size.x/2, -size.y/2}, {+size.x/2, +size.y/2}, color_, 0, size.y),
				new pline_Segment(												  color_, 0, size.x),
				new pline_Segment(												  color_, 0, size.x),
				color_,
				to_unify_colors_,
				fill_mode_,
				static_mode_)
	{}
};

class obj_ControlBox : public obj_Box
{
public:

	obj_ControlBox( double mass,
					Vec2d size,
					SmartColor *color_    = nullptr,
					bool to_unify_colors_ = true,
					int fill_mode_        = FILL,
					Vec2d  angle_pos  = Vec2d(0, 0),
					int static_mode_ = NOT_STATIC):
	obj_Box(mass,
			size,
			color_,
			to_unify_colors_,
			fill_mode_,
			angle_pos,
			static_mode_)
	{}

	void logic_tick(const double time, const double dt) {
		Vec2d mouse = eng->get_mouse_pos();
		mass_center.velocity += (mouse - mass_center.pos).normal() * 60 * dt;
	}
};

class obj_SqueezeBox : public obj_Polygon
{
public:
	Vec2d init_size;
	double time_offset;

	obj_SqueezeBox(double time_offset_,
			Vec2d size,
			SmartColor *color_    = nullptr,
			bool to_unify_colors_ = true,
			int fill_mode_        = FILL,
			Vec2d  angle_pos  = Vec2d(0, 0),
			int static_mode_ = NOT_STATIC):
	obj_Polygon(time_offset_,
				angle_pos,
				new pline_MovingSegment({-size.x/2, -size.y/2}, {-size.x/2, +size.y/2}, color_, 0, size.y, 20, 1.5, time_offset_),
				new pline_MovingSegment({+size.x/2, -size.y/2}, {+size.x/2, +size.y/2}, color_, 0, size.y, 20, 1.5, time_offset_),
				new pline_MovingSegment(												color_, 0, size.x, 10, 0),
				new pline_MovingSegment(												color_, 0, size.x, 10, 0),
				color_,
				to_unify_colors_,
				fill_mode_,
				static_mode_),
	init_size(size),
	time_offset(time_offset_)
	{}
};


const Vec2d CDG_down = {0, 1};
const Vec2d CDG_left = {-1, 0};
const Vec2d CDG_right = {1, 0};
class obj_ColorDot : public Object
{
public:
	Vec2d pos;
	SmartColor *color;
	double weight;

	obj_ColorDot(const Vec2d pos_,
				 SmartColor *color_,
				 double weight_ = 1) :
	pos(pos_),
	color(color_),
	weight(weight_)
	{}

	void logic_tick(const double time, const double dt) {
		Color cur_color = color->rgb(time, pos.x, pos.y);
		Color DELETE_COLOR(-1, -1, -1);

		PixelMap &pmap = eng->visual.pmap;
		Pixel &px = pmap.get_px(pos);
		if (px.color == DELETE_COLOR) {
			obj_delete();
			px.color *= 0;
			return;
		}

		const int grav_pos_cnt = 3;
		Vec2d grav_pos[grav_pos_cnt];
		grav_pos[0] = pos + CDG_down;
		grav_pos[1] = grav_pos[0] + CDG_right;
		grav_pos[2] = grav_pos[0] + CDG_left;

		for (int i = 0; i < grav_pos_cnt; ++i) {
			if (pmap.is_valid_coord(grav_pos[i])) {
				Pixel &gp_px = pmap.get_px(grav_pos[i]);
				if (gp_px.color.len() < 1 || gp_px.color.x < 0 || gp_px.color.y < 0 || gp_px.color.z < 0) {
					gp_px.color = cur_color;
					pos = gp_px.coord;
					break;
				}
			}
		}
		pos.x = pos.x;
		pos.y = pos.y;
		// pos.print();
		// printf("\n");
	}

	void render(PixelMap &pmap, const double current_time, const Vec2d shift = {0, 0}, const double angle_shift = 0) {
		// (pos + shift).print();
		// printf(" <\n");
		pmap.draw_pixel({pos + shift, color->rgb(current_time, pos.x + shift.x, pos.y + shift.y)});
	}
};

#endif // OBJECTS_COLLECTION