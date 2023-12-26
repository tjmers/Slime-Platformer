#include "grass.h"

#include <stdexcept>
#include <string>

#ifdef LEVEL_EDITOR
#include "../main/io_assistance.h"
#endif


Grass::Grass(const Vec2I& position, const int width, const int height)
    : Object(make_collidables(position, width, height), {}),
      position(position), width(width), height(height)
{
    if (height > MAX_HEIGHT)
        throw std::invalid_argument("height of Grass too big: " + std::to_string(height / V_UNIT) + "given, max: " + std::to_string(MAX_HEIGHT / V_UNIT));
}


void Grass::move(const Vec2I& amount)
{
    position += amount;
    Object::move(amount);
}

void Grass::draw(Graphics& g) const
{
    constexpr int ONE_WIDTH = 8_hu;

    int current_x;
    for (current_x = position.x; current_x + ONE_WIDTH <= position.x + width; current_x += ONE_WIDTH)
        g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, 128.0f, 32.0f), D2D1::RectF(current_x, position.y, current_x + ONE_WIDTH, position.y + height));
    

    if (current_x != position.x + width)
        g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, 128.0f * (static_cast<float>(position.x + width - current_x) / ONE_WIDTH), 32.0f), D2D1::RectF(current_x, position.y, position.x + width, position.y + height));


    Object::draw(g);
}

#ifdef LEVEL_EDITOR
Grass::Grass(const Grass& other)
    : Object(make_collidables(other.position, other.width, other.height), {}),
      position(other.position), width(other.width), height(other.height) {}

Object* Grass::clone() const { return new Grass(*this); }

int Grass::get_x() const { return position.x; }
int Grass::get_y() const { return position.y; }
int Grass::get_width() const { return width; }
int Grass::get_height() const { return height; }

void Grass::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << static_cast<int>(Object::TYPE::GRASS)
                << '\n' << static_cast<float>(position.x) / H_UNIT << '\n' << static_cast<float>(position.y) / V_UNIT
                << '\n' << static_cast<float>(width) / H_UNIT << '\n' <<  static_cast<float>(height) / V_UNIT;
}

void Grass::edit(StackMaxCapacity<Action, 1000>& undos)
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
        height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- bust be between 0 and 2 -- try again: ", line, 0, true, MAX_HEIGHT / V_UNIT, true) * V_UNIT);

#ifdef DRAW_HITBOXES
    collidables = make_collidables(position.x, width, position.y);
#endif

}

#endif


ID2D1Bitmap* Grass::sprite = nullptr;

HRESULT Grass::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\grass.png", &sprite);
}