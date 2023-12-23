#include "decoy.h"


ID2D1Bitmap* Decoy::sprite = nullptr;

HRESULT Decoy::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L"images\\slime.png", &sprite);
}

Decoy::Decoy(int x, int y)
    : Object({}, {}), position(x, y) {}

Decoy::Decoy(const Vec2I& position)
    : Object({}, {}), position(position) {}

void Decoy::draw(Graphics& g) const
{
    g.DrawBitmap(sprite, sprite_rect, D2D1::RectF(position.x, position.y, position.x + WIDTH, position.y + HEIGHT));
    // dont need to call Object::draw because we dont have any collidables or killables to draw
}


void Decoy::move(const Vec2I& amount)
{
    position += amount;
    // dont need to call Object::move beacuse we dont have any collidables or killables to move
}

#ifdef LEVEL_EDITOR
void Decoy::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(static_cast<int>(Object::TYPE::DECOY))
                << '\n' << static_cast<float>(position.x) / H_UNIT
                << '\n' << static_cast<float>(position.y) / V_UNIT;
}


int Decoy::get_x() const { return position.x; }
int Decoy::get_y() const { return position.y; }
int Decoy::get_width() const { return WIDTH; }
int Decoy::get_height() const { return HEIGHT; }

void Decoy::edit() {}

#endif