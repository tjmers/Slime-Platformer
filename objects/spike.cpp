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
        return { Collidable(Side::RIGHT, x + WIDTH * 0.25f, y + HEIGHT * 0.5f, HEIGHT * 0.5f), Collidable(Side::LEFT, x + WIDTH * 0.75f, y + HEIGHT * 0.5f, HEIGHT * 0.5f), Collidable(Side::BOTTOM, x + WIDTH * 0.25f, y + HEIGHT * 0.5f, HEIGHT * 0.5f) };

    case Facing::DOWN:
        return { Collidable(Side::RIGHT, x + WIDTH * 0.25f, y, HEIGHT * 0.5f), Collidable(Side::LEFT, x + WIDTH * 0.75f, y, HEIGHT * 0.5f), Collidable(Side::TOP, x + WIDTH * 0.25f, y + HEIGHT * 0.5f, HEIGHT * 0.5f) };
        
    case Facing::LEFT:
        return { Collidable(Side::BOTTOM, x + WIDTH * 0.5f, y + HEIGHT * 0.25f, WIDTH * 0.5f), Collidable(Side::TOP, x + WIDTH * 0.5f, y + HEIGHT * 0.75f, WIDTH * 0.5f), Collidable(Side::RIGHT, x + WIDTH * 0.5f, y + HEIGHT * 0.25f, HEIGHT * 0.5f) };
        
    case Facing::RIGHT:
        return { Collidable(Side::BOTTOM, x, y + HEIGHT * 0.25f, WIDTH * 0.5f), Collidable(Side::TOP, x, y + HEIGHT * 0.75f, WIDTH * 0.5f), Collidable(Side::LEFT, x + WIDTH * 0.5f, y + HEIGHT * 0.25f, HEIGHT * 0.5f) };
        
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
    g.DrawBitmap(sprites, sprite_box, D2D1::RectF(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.x + WIDTH), static_cast<float>(position.y + HEIGHT)));
    // std::cout << sprite_box.left << ' ' << sprite_box.top << ' ' << sprite_box.right << ' ' << sprite_box.bottom << '\n';
    Object::draw(g);
}


void Spike::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(Object::SPIKE)
                << '\n' << std::to_string(static_cast<int>(sprite_box.left) / 32) << '\n' << std::to_string(static_cast<float>(position.x) / H_UNIT)
                << '\n' << std::to_string(static_cast<float>(position.y) / V_UNIT);
}


int Spike::get_x() const { return position.x; }
int Spike::get_y() const { return position.y; }
int Spike::get_width() const { return WIDTH; }
int Spike::get_height() const { return HEIGHT; }