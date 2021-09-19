#ifndef GENERAL_VEC3D
#define GENERAL_VEC3D

#include <cmath>
#include <iostream>

const double VEC3_EPS = 1e-6;

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Vec3d {
    double x;
    double y;
    double z;

    Vec3d();
    Vec3d(double x_, double y_, double z_);

    double len()      const;
    Vec3d normal() const;

    double   dot  (const Vec3d &other) const;
    Vec3d cross(const Vec3d &other) const;

    Vec3d orient(const Vec3d &axis);
};

Vec3d operator+  (const Vec3d &first, const Vec3d &second);
Vec3d operator-  (const Vec3d &first, const Vec3d &second);
Vec3d operator*  (const Vec3d &first, const Vec3d &second);
Vec3d operator/  (const Vec3d &first, const Vec3d &second);

Vec3d operator*  (const Vec3d &first, const double k);
Vec3d operator/  (const Vec3d &first, const double k);

Vec3d operator+= (Vec3d &first, const Vec3d &second);
Vec3d operator*= (Vec3d &first, const Vec3d &second);
Vec3d operator*= (Vec3d &first, const double k);

bool operator==     (const Vec3d &first, const Vec3d &second);
bool operator<      (const Vec3d &first, const Vec3d &second);

std::ostream& operator<<(std::ostream& output_stream, const Vec3d &vector);
std::istream& operator>>(std::istream& input_stream, Vec3d &vector);

Vec3d oriented(Vec3d axis, Vec3d other);

Vec3d rotx(const Vec3d vec, double ang);
Vec3d roty(const Vec3d vec, double ang);
Vec3d rotz(const Vec3d vec, double ang);

Vec3d rotate(const Vec3d vec, double dx, double dy, double dz);
Vec3d rotate(const Vec3d vec, const Vec3d &rotation);

//=============================================================================
//<KCTF> Vec2 =================================================================

Vec3d::Vec3d() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Vec3d::Vec3d(double x_, double y_, double z_) {
    x = x_;
    y = y_;
    z = z_;
}

double Vec3d::len() const {
    return sqrt(x * x + y * y + z * z);
}

Vec3d Vec3d::normal() const {
    double l = len();
    if (l < VEC3_EPS) {
        return {0, 0, 0};
    } else {
        return {x / l, y / l, z / l};
    }
}

double Vec3d::dot(const Vec3d &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3d Vec3d::cross(const Vec3d &other) const {
    double res_x = y * other.z - z * other.y;
    double res_y = z * other.x - x * other.z;
    double res_z = x * other.y - y * other.x;
    return {res_x, res_y, res_z};
}

Vec3d operator+(const Vec3d &first, const Vec3d &second) {
    return {first.x + second.x, first.y + second.y, first.z + second.z};
}

Vec3d operator+=(Vec3d &first, const Vec3d &second) {
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;
    return first;
}

Vec3d operator-(const Vec3d &first, const Vec3d &second) {
    return {first.x - second.x, first.y - second.y, first.z - second.z};
}

Vec3d operator*(const Vec3d &first, const Vec3d &second) {
    return {first.x * second.x, first.y * second.y, first.z * second.z};
}

Vec3d operator*=(Vec3d &first, const Vec3d &second) {
    first.x *= second.x;
    first.y *= second.y;
    first.z *= second.z;
    return first;
}

Vec3d operator*(const Vec3d &first, const double k) {
    return {first.x * k, first.y * k, first.z * k};
}

Vec3d operator*=(Vec3d &first, const double k) {
    first.x *= k;
    first.y *= k;
    first.z *= k;
    return first;
}

Vec3d operator/(const Vec3d &first, const Vec3d &second) {
    return {first.x / second.x, first.y / second.y, first.z / second.z};
}

Vec3d operator/(const Vec3d &first, const double k) {
    return {first.x / k, first.y / k, first.z / k};
}

bool operator==(const Vec3d &first, const Vec3d &second) {
    return (first.x - second.x) < VEC3_EPS  && (first.y - second.y) < VEC3_EPS && (first.z -second.z) < VEC3_EPS;
}

bool operator<(const Vec3d &first, const Vec3d &second) {
    return first.x < second.x  && first.y < second.y && first.z < second.z;
}

std::ostream& operator<<(std::ostream& output_stream, const Vec3d &vector) {
    output_stream << vector.x << ' ' << vector.y << ' ' << vector.z;
    return output_stream;
}

std::istream& operator>>(std::istream& input_stream, Vec3d &vector) {
    char coma, bracket;
    input_stream >> bracket >> vector.x >> coma >> vector.y >> coma >> vector.z >> bracket;

    return input_stream;
}

Vec3d oriented(Vec3d axis, Vec3d other) {
    if (axis.dot(other) < 0) {
        other = other * (-1.0);
    }
    return other;
}

Vec3d Vec3d::orient(const Vec3d &axis) {
    if (axis.dot(*this) < 0) {
        *this *= -1.0;
    }
    return *this;
}

Vec3d rotx(const Vec3d vec, double ang) {
    double x = vec.x;
    double y = vec.y * cos(ang) - vec.z * sin(ang);
    double z = vec.z * cos(ang) + vec.y * sin(ang);
    return {x, y, z};
}

Vec3d roty(const Vec3d vec, double ang) {
    double x = vec.x * cos(ang) + vec.z * sin(ang);
    double y = vec.y;
    double z = vec.z * cos(ang) - vec.x * sin(ang);
    return {x, y, z};
}

Vec3d rotz(const Vec3d vec, double ang) {
    double x = vec.x * cos(ang) - vec.y * sin(ang);
    double y = vec.y * cos(ang) - vec.x * sin(ang);
    double z = vec.z;
    return {x, y, z};
}

Vec3d rotate(const Vec3d vec, double dx, double dy, double dz) {
    return rotz(roty(rotx(vec, dx), dy), dz);
}

Vec3d rotate(const Vec3d vec, const Vec3d rotation) {
    return rotz(roty(rotx(vec, rotation.x), rotation.y), rotation.z);
}

#endif // GENERAL_VEC3D