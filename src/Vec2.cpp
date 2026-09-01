#include "Vec2.h"

Vec2::Vec2() : x(0.0f), y(0.0f) {
}

Vec2::Vec2(float x, float y) : x(x), y(y) {
}

Vec2 Vec2::operator+(const Vec2& rhs) const {
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float rhs) const {
    return Vec2(x * rhs, y * rhs);
}

float Vec2::Magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::GetNormalized() const {
    float mag = Magnitude();
    if (mag != 0.0f) {
        return Vec2(x / mag, y / mag);
    }
    return Vec2(0.0f, 0.0f);
}

float Vec2::Distance(const Vec2& v2) const {
    return (*this - v2).Magnitude();
}

float Vec2::Inclination() const {
    return std::atan2(y, x);
}

float Vec2::Inclination(const Vec2& v2) const {
    return (v2 - *this).Inclination();
}

void Vec2::Rotate(float angle) {
    float s = std::sin(angle);
    float c = std::cos(angle);
    float nx = x * c - y * s;
    float ny = y * c + x * s;
    x = nx;
    y = ny;
}

Vec2 Vec2::GetRotated(float angle) const {
    Vec2 rot(x, y);
    rot.Rotate(angle);
    return rot;
}
