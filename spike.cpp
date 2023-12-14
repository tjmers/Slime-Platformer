#include "spike.h"

#include <iostream>

ID2D1Bitmap* Spike::sprites = nullptr;

Spike::Spike(Facing direction, int x, int y)
    : Object({}, make_killables(direction, x, y)), position(x, y), sprite_box(D2D1::RectF(static_cast<int>(direction) * 32, 0.0f, (static_cast<int>(direction) + 1) * 32, 32.0f)) {}

std::vector<Collidable> Spike::make_killables(Facing direction, int x, int y)
{
    switch (direction)
    {
    case Facing::UP:
        return { Collidable(Side::RIGHT, x + 0.25_hu, y + 0.5_vu, 0.5_vu), Collidable(Side::LEFT, x + 0.75_hu, y + 0.5_vu, 0.5_vu), Collidable(Side::BOTTOM, static_cast<int>(x + 0.25_hu), y + 0.5_vu, 0.5_hu) };

    case Facing::DOWN:
        return { Collidable(Side::RIGHT, x + 0.25_hu, y, 0.5_vu), Collidable(Side::LEFT, x + 0.75_hu, y, 0.5_vu), Collidable(Side::TOP, static_cast<int>(x + 0.25_hu), y + 0.5_vu, 0.5_hu) };
        
    case Facing::LEFT:
        return { Collidable(Side::BOTTOM, x + 0.5_hu, y + 0.25_vu, 0.5_hu), Collidable(Side::TOP, x + 0.5_hu, 0 + 0.75_vu, 0.5_hu), Collidable(Side::RIGHT, x + 0.5_hu, y + 0.25_vu, 0.5_vu) };
        
    case Facing::RIGHT:
        return { Collidable(Side::BOTTOM, x, y + 0.25_vu, 0.5_hu), Collidable(Side::TOP, x, 0 + 0.75_vu, 0.5_hu), Collidable(Side::LEFT, x + 0.5_hu, y + 0.25_vu, 0.5_vu) };
        
    default:
        throw std::runtime_error("something went wrong");
    }
}

HRESULT Spike::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\spikes.png", &sprites);
}

void Spike::move(const Vec2I& amount)
{
    // std::cout << "MOVING\n";
    Object::move(amount);
    position += amount;
}

void Spike::draw(Graphics& g) const
{
    // std::cout << position.x << ' ' << position.y << '\n';
    g.DrawBitmap(sprites, sprite_box, D2D1::RectF(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.x + 1_hu), static_cast<float>(position.y + 1_hu)));
    // std::cout << sprite_box.left << ' ' << sprite_box.top << ' ' << sprite_box.right << ' ' << sprite_box.bottom << '\n';
    Object::draw(g);
}