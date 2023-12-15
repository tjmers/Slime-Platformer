#pragma once

#include <vector>

#include "../main/object.h"


class WoodenFloor : public Object
{
public:
    WoodenFloor(const int& x1, const int& x2, const int& y, const int& height);
    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;
    static HRESULT init(Graphics& g);
private:
    Vec2I position;
    int width, height;

    static std::vector<Collidable> make_collidables(const int& x1, const int& x2, const int& y, const int& height);
    static ID2D1Bitmap* sprite;
    constexpr static int sprite_width = 128, sprite_height = 32;
};