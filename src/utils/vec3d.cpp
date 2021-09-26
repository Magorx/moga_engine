#include "vec3d.h"

const Vec3d VEC3D_ZERO(0, 0, 0);
const Vec3d VEC3D_ONE(1, 1, 1);

long vec3d_randlong() {
    long ret = rand();
    ret |= rand() << sizeof(int);
    return ret > 0 ? ret : -ret;
}

double vec3d_randdouble(double dmin, double dmax) {
    double x = (double) rand() / RAND_MAX;
    return dmin + x * (dmax - dmin);
}

Vec3d::Vec3d() {
    content = {0, 0, 0};
}

Vec3d::Vec3d(double x_, double y_, double z_) {
    content = {x_, y_, z_};
}

[[maybe_unused]] Vec3d Vec3d::orient(const Vec3d &axis) {
    if(axis.dot(*this) < 0) {
        *this *= -1.0;
    }
    return *this;
}

Vec3d &Vec3d::abs() {
    content[0] = fabs(content[0]);
    content[1] = fabs(content[1]);
    content[2] = fabs(content[2]);

    return *this;
}

Vec3d::Vec3d(content3 newContent) : content(newContent) {}

Vec3d rotx(const Vec3d vec, double ang) {
    double x = vec.content[0];
    double y = vec.content[1] * cos(ang) - vec.content[2] * sin(ang);
    double z = vec.content[2] * cos(ang) + vec.content[1] * sin(ang);
    return {x, y, z};
}

Vec3d roty(const Vec3d vec, double ang) {
    double x = vec.content[0] * cos(ang) + vec.content[2] * sin(ang);
    double y = vec.content[1];
    double z = vec.content[2] * cos(ang) - vec.content[0] * sin(ang);
    return {x, y, z};
}

Vec3d rotz(const Vec3d vec, double ang) {
    double x = vec.content[0] * cos(ang) + vec.content[1] * sin(ang);
    double y = vec.content[1] * cos(ang) - vec.content[0] * sin(ang);
    double z = vec.content[2];
    return {x, y, z};
}

Vec3d rotate(const Vec3d vec, double dx, double dy, double dz) {
    return rotz(roty(rotx(vec, dx), dy), dz);
}

Vec3d rotate(const Vec3d vec, const Vec3d rotation) {
    return rotz(roty(rotx(vec, rotation.content[0]), rotation.content[1]), rotation.content[2]);
}

Vec3d refract(const Vec3d vec, const Vec3d &normal, const double eta_from_over_eta_to) {
    Vec3d uv = vec.normal();
    Vec3d n = normal.normal();

    auto cos_theta = fmin(-uv.dot(n), 1.0);
    Vec3d r_out_perp = eta_from_over_eta_to * (uv + cos_theta * n);
    Vec3d r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.len_squared())) * n;
    return r_out_perp + r_out_parallel;
}

double vec3d_deg_to_rad(const double deg) {
    return deg / 180 * VEC3D_PI;
}

double vec3d_rad_to_deg(const double rad) {
    return rad / VEC3D_PI * 180;
}
