#include "interpolator.h"


void Interpolator2d::add_to_data(const Vec2d point) {
    if (!data.size()) {
        data.push_back(point);
        return;
    }

    if (point.x() < data[0].x()) {
        data.insert(data.begin(), point);
        return;
    }

    if (point.x() > data[data.size() - 1].x()) {
        data.push_back(point);
    }

    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].x() > point.x()) {
            data.insert(data.begin() + i, point);
            break;
        }
    }
}

Interpolator2d::Interpolator2d(bool to_swap_coords, size_t init_len) :
data(),
swap_coords(to_swap_coords)
{
    data.reserve(init_len);
}

void Interpolator2d::add(Vec2d point) {
    if (swap_coords) {
        double tmp = point.content[0];
        point.content[0] = point.content[1];
        point.content[1] = tmp;
    }

    add_to_data(point);
}

double Interpolator2d::get_t(double t, double alpha, const Vec2d& p1, const Vec2d& p2) {
    auto d  = p2 - p1;
    double a = d.dot(d);
    double b = std::pow(a, alpha * 0.5);
    return (b + t);
}

#define VF "(%g, %g)"
#define V(v) v.x(), v.y()

Vec2d Interpolator2d::get_y(const Vec2d &p1, const Vec2d &p2, const Vec2d &p3, const Vec2d &p4, double t, double alpha) {
    // printf("pts " VF " " VF " " VF " " VF, V(p1), V(p2), V(p3), V(p4));
    double t1 = 0.0f;
    double t2 = get_t( t1, alpha, p1, p2 );
    double t3 = get_t( t2, alpha, p2, p3 );
    double t4 = get_t( t3, alpha, p3, p4 );

    //printf("t1 = %f, t2 = %f, t3 = %f, t4 = %f\n", t1, t2, t3, t4);

    t = std::lerp(t2, t3, t);
    Vec2d A1 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
    Vec2d A2 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;
    Vec2d A3 = (t4 - t) / (t4 - t3) * p3 + (t - t3) / (t4 - t3) * p4;
    Vec2d B1 = (t3 - t) / (t3 - t1) * A1 + (t - t1) / (t3 - t1) * A2;
    Vec2d B2 = (t4 - t) / (t4 - t2) * A2 + (t - t2) / (t4 - t2) * A3;
    Vec2d C  = (t3 - t) / (t3 - t2) * B1 + (t - t2) / (t3 - t2) * B2;
    
    return C;
}

Vec2d Interpolator2d::operator[](double x) {
    size_t data_size = data.size();

    if (!data_size) {
        return NAN;
    }

    if (x < data[0].x()) {
        return data[0].y();
    }

    if (x > data[data_size - 1].x()) {
        // printf("x %g minx %g\n", x, data[0].x());
        return data[data_size - 1].y();
    }

    if (data_size < 4) {
        return NAN;
    }

    for (size_t i = 1; i < data_size - 2; ++i) {
        if (data[i].x() <= x && x <= data[i + 1].x()) {
            double t = (x - data[i].x()) / (data[i + 1].x() - data[i].x());
            return get_y(data[i - 1], data[i], data[i + 1], data[i + 2], t);
        }
    }

    return NAN;
}
