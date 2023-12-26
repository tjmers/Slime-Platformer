#include "wooden_floor.h"

#ifdef LEVEL_EDITOR
#include "../main/io_assistance.h"
#endif


ID2D1Bitmap* WoodenFloor::sprite = nullptr;

WoodenFloor::WoodenFloor(const int& x, const int& y, const int& width, const int& height)
    : Object(make_collidables(x, width, y), std::vector<Collidable>()), position(x, y), width(width), height(height) {}

std::vector<Collidable> WoodenFloor::make_collidables(const int& x1, const int& width, const int& y)
{
    return {Collidable(Side::BOTTOM, x1, y, width)};
}

void WoodenFloor::move(const Vec2I& amount)
{
    Object::move(amount);
    position += amount;
}

void WoodenFloor::draw(Graphics&g) const
{
    constexpr static int ONE_WIDTH = 8_hu, ONE_HEIGHT = 2_hu;
    static_assert(ONE_WIDTH / ONE_HEIGHT == sprite_width / sprite_height);
    for (int current_x = position.x; current_x < position.x + width && current_x < SCREEN_WIDTH; current_x += ONE_WIDTH)
    {
        if (current_x + ONE_WIDTH < 0)
            continue;
        int x2 = std::min(current_x + ONE_WIDTH, position.x + width);

        for (int current_y = position.y; current_y < position.y + height && current_y < SCREEN_HEIGHT; current_y += ONE_HEIGHT)
        {
            if (current_y + ONE_HEIGHT < 0)
                continue;
            int y2 = std::min(current_y + ONE_HEIGHT, position.y + height);
            if (x2 != current_x + ONE_WIDTH && y2 != current_y + ONE_HEIGHT)
                g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, sprite_width * (static_cast<float>(position.x + width - current_x) / ONE_WIDTH), sprite_height * (static_cast<float>(position.y + height - current_y) / ONE_HEIGHT)), D2D1::RectF(current_x, current_y, x2, y2));
            else if (x2 != current_x + ONE_WIDTH)
                g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, sprite_width * (static_cast<float>(position.x + width - current_x) / ONE_WIDTH), sprite_height), D2D1::RectF(current_x, current_y, x2, y2));
            else if (y2 != current_y + ONE_HEIGHT)
                g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, sprite_width, sprite_height * (static_cast<float>(position.y + height - current_y) / ONE_HEIGHT)), D2D1::RectF(current_x, current_y, x2, y2));
            else
                g.DrawBitmap(sprite, D2D1::RectF(0.0f, 0.0f, sprite_width, sprite_height), D2D1::RectF(current_x, current_y, x2, y2));
        }
    }
    
    Object::draw(g);
}

HRESULT WoodenFloor::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\wooden_floor.png", &sprite);
}

#ifdef LEVEL_EDITOR

WoodenFloor::WoodenFloor(const WoodenFloor& other)
    : Object(make_collidables(other.position.x, other.width, other.position.y), {}), position(other.position), width(other.width), height(other.height) {}

Object* WoodenFloor::clone() const { return new WoodenFloor(*this); }

void WoodenFloor::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(static_cast<int>(Object::TYPE::WOODEN_FLOOR))
                << '\n' << std::to_string(static_cast<float>(position.x) / H_UNIT) << '\n' << std::to_string(static_cast<float>(position.y) / V_UNIT)
                << '\n' << std::to_string(static_cast<float>(width) / H_UNIT) << '\n' << std::to_string(static_cast<float>(height) / V_UNIT);
}

int WoodenFloor::get_x() const { return position.x; }
int WoodenFloor::get_y() const { return position.y; }
int WoodenFloor::get_width() const { return width; }
int WoodenFloor::get_height() const { return height; }

void WoodenFloor::edit(StackMaxCapacity<Action, 1000>& undos)
{
    undos.push(Action(
        [this] (Action::Param& old_width_and_height)
        {
            width = old_width_and_height.four_byte1;
            height = old_width_and_height.four_byte2;
        #ifdef DRAW_HITBOXES
            collidables = make_collidables(position.x, width, position.y);
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