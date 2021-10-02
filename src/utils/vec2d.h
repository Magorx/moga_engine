#pragma once

#include "vec3d.h"

class Vec2d : public Vec3d {
public:
    using Vec3d::len;
    using Vec3d::len_squared;
    using Vec3d::normal;
    using Vec3d::normalize;
    using Vec3d::is_zero;
    using Vec3d::dot;
    using Vec3d::abs;
    using Vec3d::set;

    using Vec3d::x;
    using Vec3d::y;
    using Vec3d::z;

    Vec2d(content3 newContent) {
        content = newContent;
    }

    Vec2d(double x, double y) {
        content[0] = x;
        content[1] = y;
    }

    Vec2d(const Vec3d &vec3) {
        content[0] = vec3.content[0];
        content[1] = vec3.content[1];
        content[2] = 1;
        content[3] = 0;
    }
};


inline Vec2d operator+(const Vec2d &first) {
    return first;
}

inline Vec2d operator-(const Vec2d &first) {
    return {first.content * -1};
}

inline Vec2d operator+(const Vec2d &first, const Vec2d &second) {
    return {first.content + second.content};
}

inline Vec2d operator-(const Vec2d &first, const Vec2d &second) {
    return {first.content - second.content};
}

inline Vec2d operator*(const Vec2d &first, const Vec2d &second) {
    return {first.content * second.content};
}

inline Vec2d operator/(const Vec2d &first, const Vec2d &second) {
    return {first.content / second.content};
}

inline Vec2d operator*(const Vec2d &first, const double k) {
    return {first.content * k};
}

inline Vec2d operator*(const double k, const Vec2d &first) {
    return {first.content * k};
}

inline Vec2d operator/(const Vec2d &first, const double k) {
    return {first.content / k};
}

inline Vec2d &operator+=(Vec2d &first, const Vec2d &second) {
    first.content += second.content;
    return first;
}

inline Vec2d &operator-=(Vec2d &first, const Vec2d &second) {
    first.content -= second.content;
    return first;
}

inline Vec2d &operator*=(Vec2d &first, const Vec2d &second) {
    first.content *= second.content;
    return first;
}

inline Vec2d &operator*=(Vec2d &first, const double k) {
    first.content *= k;
    return first;
}

inline Vec2d &operator/=(Vec2d &first, const Vec2d &second) {
    first.content /= second.content;
    return first;
}

inline Vec2d &operator/=(Vec2d &first, const double k) {
    first.content /= k;
    return first;
}

inline bool operator==(const Vec2d &first, const Vec2d &second) {
    const auto res = first.content - second.content;
    return (res[0]) < VEC3_EPS && (res[1]) < VEC3_EPS && (res[2]) < VEC3_EPS;
}

inline bool operator<(const Vec2d &first, const Vec2d &second) {
    const auto res = first.content < second.content;
    return res[0] * res[1] * res[2];
}

inline Vec2d sqrt(const Vec2d &first) {
    return {sqrt(first.content[0]),
            sqrt(first.content[1])};
}

inline Vec2d pow(const Vec2d &first, const double power) {
    return {pow(first.content[0], power),
            pow(first.content[1], power)};
}
