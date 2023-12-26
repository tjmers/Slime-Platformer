#include "dirt.h"

#include <string>

#ifdef LEVEL_EDITOR
#include "../main/io_assistance.h"
#endif


Dirt::Dirt(const Vec2I& position, const int width, const int height)
    : Object(make_collidables(position, width, height), {}),
      position(position), width(width), height(height)
{}


void Dirt::move(const Vec2I& amount)
{
    position += amount;
    Object::move(amount);
}

void Dirt::draw(Graphics& g) const
{
    constexpr int ONE_WIDTH = 2_hu;
    constexpr int ONE_HEIGHT = 2_vu;
    static_assert(ONE_WIDTH / ONE_HEIGHT == SPRITE_WIDTH / SPRITE_HEIGHT);

    int current_x;
    for (current_x = position.x; current_x + ONE_WIDTH <= position.x + width && current_x < SCREEN_WIDTH; current_x += ONE_WIDTH)
    {
        if (current_x + ONE_WIDTH < 0)
            continue;
        int current_y;
        for (current_y = position.y; current_y + ONE_HEIGHT <= position.y + height && position.y < SCREEN_HEIGHT; current_y += ONE_HEIGHT)
        {
            if (current_y + ONE_HEIGHT < 0)
                continue;
            g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, SPRITE_WIDTH, SPRITE_HEIGHT), D2D1::RectF(current_x, current_y, current_x + ONE_WIDTH, current_y + ONE_HEIGHT));
        }
        
        if (current_y != position.y + height)
            g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, SPRITE_WIDTH, SPRITE_HEIGHT * (static_cast<float>(position.y + height - current_y) / ONE_HEIGHT)), D2D1::RectF(current_x, current_y, current_x + ONE_WIDTH, position.y + height));

    }

    
    if (current_x != position.x + width)
    {
        int current_y;
        for (current_y = position.y; current_y + ONE_HEIGHT <= position.y + height; current_y += ONE_HEIGHT)
            g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, SPRITE_WIDTH * (static_cast<float>(position.x + width - current_x) / ONE_WIDTH), SPRITE_HEIGHT), D2D1::RectF(current_x, current_y, position.x + width, current_y + ONE_HEIGHT));

        if (current_y != position.y + height)
            g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, SPRITE_WIDTH * (static_cast<float>(position.x + width - current_x) / ONE_WIDTH), SPRITE_HEIGHT * (static_cast<float>(position.y + height - current_y) / ONE_HEIGHT)), D2D1::RectF(current_x, current_y, position.x + width, position.y + height));
    }

    Object::draw(g);
}

#ifdef LEVEL_EDITOR
Dirt::Dirt(const Dirt& other)
    : Object(make_collidables(other.position, other.width, other.height), {}),
      position(other.position), width(other.width), height(other.height) {}

Object* Dirt::clone() const { return new Dirt(*this); }

int Dirt::get_x() const { return position.x; }
int Dirt::get_y() const { return position.y; }
int Dirt::get_width() const { return width; }
int Dirt::get_height() const { return height; }

void Dirt::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << static_cast<int>(Object::TYPE::DIRT)
                << '\n' << static_cast<float>(position.x) / H_UNIT << '\n' << static_cast<float>(position.y) / V_UNIT
                << '\n' << static_cast<float>(width) / H_UNIT << '\n' << static_cast<float>(height) / V_UNIT;
}

void Dirt::edit(StackMaxCapacity<Action, 1000>& undos)
{
    undos.push(Action(
        [this] (Action::Param& old_width_and_height)
        {
            width = old_width_and_height.four_byte1;
            height = old_width_and_height.four_byte2;
        #ifdef DRAW_HITBOXES
            collidables = make_collidables(position.x, position.y, width);
        #endif
        }, Action::Param(width, height)
    ));
    std::string line;
    std::cout << "Width (" << static_cast<float>(width / H_UNIT) << "): ";
    std::cin >> line;
    
    if (line != "-")
        width = static_cast<int>(IoAssistance::get_valid_float("Invalid width -- try again: ", line) * H_UNIT);
    

    std::cout << "Height (" << static_cast<float>(height / V_UNIT) << "): ";
    std::cin >> line;

    if (line != "-")
        height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- try again: ", line) * V_UNIT);

#ifdef DRAW_HITBOXES
    collidables = make_collidables(position.x, width, position.y);
#endif

}

#endif


ID2D1Bitmap* Dirt::sprite = nullptr;

HRESULT Dirt::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\dirt.png", &sprite);
}