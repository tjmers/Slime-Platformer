#include <cmath>

#include "vec2.h"

Vec2F::Vec2F() : x(0.0f), y(0.0f) {}

Vec2F::Vec2F(const float x, const float y)
    : x(x), y(y) {}

Vec2F::Vec2F(const Vec2F& other)
    : x(other.x), y(other.y) {}
    

Vec2F Vec2F::operator+(const Vec2F other) const
{
    return Vec2F(x + other.x, y + other.y);
}

Vec2F Vec2F::operator-(const Vec2F other) const
{
    return Vec2F(x - other.x, y - other.y);
}

Vec2F Vec2F::operator*(const float& scalar) const
{
    return Vec2F(x * scalar, y * scalar);
}

Vec2F Vec2F::operator/(const float& scalar) const
{
    return Vec2F(x / scalar, y / scalar);
}

void Vec2F::operator+=(const Vec2F& other)
{
    x += other.x;
    y += other.y;
}

void Vec2F::operator+=(const Vec2I& iVec)
{
    x += iVec.x;
    y += iVec.y;
}

void Vec2F::operator-=(const Vec2F& other)
{
    x -= other.x;
    y -= other.y;
}

void Vec2F::operator*=(const float& scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vec2F::operator/=(const float& scalar)
{
    x /= scalar;
    y /= scalar;
}





Vec2I::Vec2I() : x(0), y(0) {}

Vec2I::Vec2I(const int x, const int y)
    : x(x), y(y) {}

Vec2I::Vec2I(const Vec2I& other)
    : x(other.x), y(other.y) {}
    

Vec2I Vec2I::operator+(const Vec2I other) const
{
    return Vec2I(x + other.x, y + other.y);
}

Vec2I Vec2I::operator-(const Vec2I other) const
{
    return Vec2I(x - other.x, y - other.y);
}

Vec2I Vec2I::operator*(const int& scalar) const
{
    return Vec2I(x * scalar, y * scalar);
}

Vec2I Vec2I::operator/(const int& scalar) const
{
    return Vec2I(x / scalar, y / scalar);
}

void Vec2I::operator+=(const Vec2I& other)
{
    x += other.x;
    y += other.y;
}

void Vec2I::operator-=(const Vec2I& other)
{
    x -= other.x;
    y -= other.y;
}

void Vec2I::operator*=(const int& scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vec2I::operator/=(const int& scalar)
{
    x /= scalar;
    y /= scalar;
}

Vec2I::operator bool() const
{
    return x != 0 || y != 0;
}