#pragma once

#include <cmath>

class Vec2 {
public:
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);

    Vec2 operator+(const Vec2& rhs) const;
    Vec2 operator-(const Vec2& rhs) const;
    Vec2 operator*(const float rhs) const;

    float Magnitude() const;
    Vec2 GetNormalized() const;
    float Distance(const Vec2& v2) const;
    float Inclination() const;
    float Inclination(const Vec2& v2) const;
    void Rotate(float angle);
    Vec2 GetRotated(float angle) const;
};
