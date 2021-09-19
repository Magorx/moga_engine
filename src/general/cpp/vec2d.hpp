#ifndef GENERAL_VEC2D
#define GENERAL_VEC2D

#include <cmath>
#include <iostream>

const double VEC2_EPS = 1e-6;

//=============================================================================
//<KCTF> Vec2_header ==========================================================

struct Vec2d {
    double x;
    double y;

    Vec2d();
    Vec2d(double x_, double y_);

    double  len      () const;
    Vec2d   normal   () const;
    Vec2d  &normalize()      ;

    Vec2d &nullify();

    double dot  (const Vec2d &other) const;
    double cross(const Vec2d &other) const;

    Vec2d orient(const Vec2d &axis);
    double angle(const Vec2d  axis);

    void print(FILE *file_ptr = stdout);
    void dump (FILE *file_ptr = stdout);
};

Vec2d operator-  (const Vec2d &first);
Vec2d operator+  (const Vec2d &first, const Vec2d &second);
Vec2d operator-  (const Vec2d &first, const Vec2d &second);
Vec2d operator*  (const Vec2d &first, const Vec2d &second);
Vec2d operator/  (const Vec2d &first, const Vec2d &second);

Vec2d operator*  (const Vec2d &first, const double k);
Vec2d operator/  (const Vec2d &first, const double k);

Vec2d &operator+= (Vec2d &first, const Vec2d &second);
Vec2d &operator-= (Vec2d &first, const Vec2d &second);
Vec2d &operator*= (Vec2d &first, const Vec2d &second);
Vec2d &operator*= (Vec2d &first, const double k);
Vec2d &operator/= (Vec2d &first, const double k);

bool operator==     (const Vec2d &first, const Vec2d &second);
bool operator<      (const Vec2d &first, const Vec2d &second);

std::ostream& operator<<(std::ostream& output_stream, const Vec2d &vector);
std::istream& operator>>(std::istream& input_stream, Vec2d &vector);

Vec2d oriented(Vec2d axis, Vec2d other);

Vec2d rot_clockwise        (const Vec2d vec, double ang);
Vec2d rot_counterclockwise (const Vec2d vec, double ang);

Vec2d rotate(const Vec2d vec, double dx, double dy, double dz);
Vec2d rotate(const Vec2d vec, const Vec2d &rotation);

double vec2d_rand_double(double dmin, double dmax) {
    double x = (double)rand() / RAND_MAX;
    return dmin + x * (dmax - dmin);
}

// pretty random functions

Vec2d random_uni_vector() {
    return Vec2d(vec2d_rand_double(-1, 1), vec2d_rand_double(-1, 1)).normal();
}

double vec2d_sign(double x) {
    if (fabs(x) < VEC2_EPS) {
        return 0;
    } else {
        if (x > 0) {
            return 1;
        } else {
            return -1;
        }
    }
}

double segment_point_distance(const Vec2d a, const Vec2d b, const Vec2d p) {
    Vec2d a_b = b - a;
    double a_b_len = a_b.len();
    a_b.normalize();

    Vec2d proec = a + a_b * a_b.dot(p - a);

    if ((proec - a).dot(a_b) * (proec - b).dot(a_b) > 0) {
        return std::min((a - p).len(), (b - p).len());
    } else {
        return (proec - p).len();
    }
}

//=============================================================================
//<KCTF> Vec2 =================================================================

Vec2d::Vec2d() {
    x = 0.0;
    y = 0.0;
}

Vec2d::Vec2d(double x_, double y_) {
    x = x_;
    y = y_;
}

double Vec2d::len() const {
    return sqrt(x * x + y * y);
}

Vec2d Vec2d::normal() const {
    double l = len();
    if (l < VEC2_EPS) {
        return {0, 0};
    } else {
        return {x / l, y / l};
    }
}

Vec2d &Vec2d::normalize() {
    double l = len();
    if (l < VEC2_EPS) {
        x = 0;
        y = 0;
    } else {
        x /= l;
        y /= l;
    }
    return *this;
}

Vec2d &Vec2d::nullify() {
    x = 0;
    y = 0;
    return *this;
}

double Vec2d::dot(const Vec2d &other) const {
    return x * other.x + y * other.y;
}

double Vec2d::cross(const Vec2d &other) const {
    return x * other.y - y * other.x;
}

Vec2d operator-(const Vec2d &first) {
    return {-first.x, -first.y};
}

Vec2d operator+(const Vec2d &first, const Vec2d &second) {
    return {first.x + second.x, first.y + second.y};
}

Vec2d operator-(const Vec2d &first, const Vec2d &second) {
    return {first.x - second.x, first.y - second.y};
}

Vec2d operator*(const Vec2d &first, const Vec2d &second) {
    return {first.x * second.x, first.y * second.y};
}

Vec2d &operator+=(Vec2d &first, const Vec2d &second) {
    first.x += second.x;
    first.y += second.y;
    return first;
}

Vec2d &operator-=(Vec2d &first, const Vec2d &second) {
    first.x -= second.x;
    first.y -= second.y;
    return first;
}

Vec2d &operator*=(Vec2d &first, const Vec2d &second) {
    first.x *= second.x;
    first.y *= second.y;
    return first;
}

Vec2d operator*(const Vec2d &first, const double k) {
    return {first.x * k, first.y * k};
}

Vec2d &operator*=(Vec2d &first, const double k) {
    first.x *= k;
    first.y *= k;
    return first;
}

Vec2d &operator/=(Vec2d &first, const double k) {
    first.x /= k;
    first.y /= k;
    return first;
}

Vec2d operator/(const Vec2d &first, const Vec2d &second) {
    return {first.x / second.x, first.y / second.y};
}

Vec2d operator/(const Vec2d &first, const double k) {
    return {first.x / k, first.y / k};
}

bool operator==(const Vec2d &first, const Vec2d &second) {
    return (first.x - second.x) < VEC2_EPS  && (first.y - second.y) < VEC2_EPS;
}

bool operator<(const Vec2d &first, const Vec2d &second) {
    return first.x < second.x  && first.y < second.y;
}

std::ostream& operator<<(std::ostream& output_stream, const Vec2d &vector) {
    output_stream << vector.x << ' ' << vector.y;
    return output_stream;
}

std::istream& operator>>(std::istream& input_stream, Vec2d &vector) {
    char coma, bracket;
    input_stream >> bracket >> vector.x >> coma >> vector.y >> bracket;

    return input_stream;
}

Vec2d oriented(Vec2d axis, Vec2d other) {
    if (axis.dot(other) < 0) {
        other = other * (-1.0);
    }
    return other;
}

Vec2d Vec2d::orient(const Vec2d &axis) {
    if (axis.dot(*this) < 0) {
        *this *= -1.0;
    }
    return *this;
}

double Vec2d::angle(const Vec2d axis) {
    double dot = x * axis.x + y * axis.y;
    double det = x * axis.y - y * axis.x;
    return -atan2(det, dot);
}

Vec2d rot_counterclockwise(const Vec2d vec, double ang) {
    double x = vec.x * cos(ang) - vec.y * sin(ang);
    double y = vec.x * sin(ang) + vec.y * cos(ang);
    return {x, y};
}

Vec2d rot_clockwise(const Vec2d vec, double ang) {
    double x = vec.x * cos(ang) + vec.y * sin(ang);
    double y = -vec.x * sin(ang) + vec.y * cos(ang);
    return {x, y};
}


void Vec2d::print(FILE *file_ptr) {
    fprintf(file_ptr, "%lg %lg", x, y);
}

void Vec2d::dump(FILE *file_ptr) {
    fprintf(file_ptr, "[vec2d %lg %lg]", x, y);
}

#endif // GENERAL_VEC2D