#pragma once

struct Vec2I;
struct Vec2F
{

public:
    float x, y;

    Vec2F();
    Vec2F(const float x, const float y);
    Vec2F(const Vec2F& other);

    Vec2F operator+(const Vec2F other) const;
    Vec2F operator-(const Vec2F other) const;
    Vec2F operator*(const float& scalar) const;
    Vec2F operator/(const float& scalar) const;
    void operator+=(const Vec2F& other);
    void operator+=(const Vec2I& iVec);
    void operator-=(const Vec2F& other);
    void operator*=(const float& scalar);
    void operator/=(const float& scalar);

    operator Vec2I() const;
};

struct Vec2I
{

public:
    int x, y;

    Vec2I();
    Vec2I(const int x, const int y);
    Vec2I(const Vec2I& other);

    Vec2I operator+(const Vec2I other) const;
    Vec2I operator-(const Vec2I other) const;
    Vec2I operator*(const int& scalar) const;
    Vec2I operator/(const int& scalar) const;
    void operator+=(const Vec2I& other);
    void operator-=(const Vec2I& other);
    void operator*=(const int& scalar);
    void operator/=(const int& scalar);

    operator bool() const;

    operator Vec2F() const;
};