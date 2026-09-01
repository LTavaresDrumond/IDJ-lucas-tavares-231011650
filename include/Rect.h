#pragma once

#include "Vec2.h"

class Rect {
public:
    float x;
    float y;
    float w;
    float h;

    Rect();
    Rect(float x, float y, float w, float h);

    bool Contains(float pX, float pY) const;
    bool Contains(const Vec2& p) const;

    Vec2 GetCenter() const;
    void SetCenter(float x, float y);
    void SetCenter(const Vec2& p);

    float Distance(const Rect& r2) const;

    Rect operator+(const Vec2& rhs) const;
    Rect operator-(const Vec2& rhs) const;
    Rect& operator+=(const Vec2& rhs);
    Rect& operator-=(const Vec2& rhs);
};
