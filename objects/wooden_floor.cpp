#include "wooden_floor.h"

ID2D1Bitmap* WoodenFloor::sprite = nullptr;

WoodenFloor::WoodenFloor(const int& x, const int& y, const int& width, const int& height)
    : Object(make_collidables(x, x + width, y, height), std::vector<Collidable>()), position(x, y), width(width), height(height) {}

std::vector<Collidable> WoodenFloor::make_collidables(const int& x1, const int& x2, const int& y, const int& height)
{
    return {Collidable(Side::BOTTOM, x1, y, x2 - x1)};
}

void WoodenFloor::move(const Vec2I& amount)
{
    Object::move(amount);
    position += amount;
}

void WoodenFloor::draw(Graphics&g) const
{
    for (int current_x = position.x; current_x < position.x + width && current_x < SCREEN_WIDTH; current_x += 4_vu)
    {
        if (current_x + 4_vu < 0)
            continue;
        for (int current_y = position.y; current_y < position.y + height && current_y < SCREEN_HEIGHT; current_y += 1_hu)
        {
            if (current_y + 1_hu < 0)
                continue;
            g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, sprite_width, sprite_height), D2D1::RectF(current_x, current_y, current_x + sprite_width, current_y + sprite_height));
        }
    }
    
    Object::draw(g);
}

HRESULT WoodenFloor::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\wooden_floor.png", &sprite);
}


void WoodenFloor::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(static_cast<int>(Object::TYPE::WOODEN_FLOOR))
                << '\n' << std::to_string(static_cast<float>(position.x) / H_UNIT) << '\n' << std::to_string(static_cast<float>(width) / H_UNIT)
                << '\n' << std::to_string(static_cast<float>(position.y) / V_UNIT) << '\n' << std::to_string(static_cast<float>(height) / V_UNIT);
}


int WoodenFloor::get_x() const { return position.x; }
int WoodenFloor::get_y() const { return position.y; }
int WoodenFloor::get_width() const { return width; }
int WoodenFloor::get_height() const { return height; }