#include "Rect.h"

Rect::Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {
}

Rect::Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {
}

bool Rect::Contains(float pX, float pY) const {
    return (pX >= x && pX <= (x + w) && pY >= y && pY <= (y + h));
}

bool Rect::Contains(const Vec2& p) const {
    return Contains(p.x, p.y);
}

Vec2 Rect::GetCenter() const {
    return Vec2(x + w / 2.0f, y + h / 2.0f);
}

void Rect::SetCenter(float cx, float cy) {
    x = cx - w / 2.0f;
    y = cy - h / 2.0f;
}

void Rect::SetCenter(const Vec2& p) {
    SetCenter(p.x, p.y);
}

float Rect::Distance(const Rect& r2) const {
    return GetCenter().Distance(r2.GetCenter());
}

Rect Rect::operator+(const Vec2& rhs) const {
    return Rect(x + rhs.x, y + rhs.y, w, h);
}

Rect Rect::operator-(const Vec2& rhs) const {
    return Rect(x - rhs.x, y - rhs.y, w, h);
}

Rect& Rect::operator+=(const Vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Rect& Rect::operator-=(const Vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
