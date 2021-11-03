#pragma once


#include <vector>
#include <cmath>

#include "vec2d.h"


class Interpolator2d {
    void add_to_data(const Vec2d point);

public:
    std::vector<Vec2d> data;
    bool swap_coords;

    Interpolator2d(bool to_swap_coords = false, size_t init_len = 8);

    void add(Vec2d point);

    double get_t(double t, double alpha, const Vec2d& p1, const Vec2d& p2);

    Vec2d get_y(const Vec2d &p1, const Vec2d &p2, const Vec2d &p3, const Vec2d &p4, double t, double alpha = 0.5);

    Vec2d operator[](double x);
};
