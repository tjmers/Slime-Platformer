#include "stone_wall.h"

#ifdef LEVEL_EDITOR
#include "../main/io_assistance.h"
#endif


ID2D1Bitmap* StoneWall::sprite = nullptr;

StoneWall::StoneWall(const Vec2I& position, const int width, const int height)
    : Object({}, {}), position(position), width(width), height(height) {}


void StoneWall::move(const Vec2I& amount)
{
    position += amount; // no need to call Object::move since we dont have any collidables or killables. Same goes for StoneWall::draw(Graphics& g)
}

void StoneWall::draw(Graphics& g) const
{
    constexpr static int ONE_WIDTH = 2_hu, ONE_HEIGHT = 2_hu;
    constexpr static int SPRITE_WIDTH = 32, SPRITE_HEIGHT = 32;
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
    
}


HRESULT StoneWall::init(Graphics& g)
{
    return g.LoadBitmapFromFile(L".\\images\\stone_wall.png", &sprite);
}


#ifdef LEVEL_EDITOR

StoneWall::StoneWall(const StoneWall& other)
    : Object({}, {}),
      position(other.position), width(other.width), height(other.height) {}

Object* StoneWall::clone() const { return new StoneWall(*this); }

int StoneWall::get_x() const { return position.x; }
int StoneWall::get_y() const { return position.y; }
int StoneWall::get_width() const { return width; }
int StoneWall::get_height() const { return height; }

void StoneWall::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(static_cast<int>(Object::TYPE::STONE_WALL))
                << '\n' << std::to_string(static_cast<float>(position.x) / H_UNIT) << '\n' << std::to_string(static_cast<float>(position.y) / V_UNIT)
                << '\n' << std::to_string(static_cast<float>(width) / H_UNIT) << '\n' << std::to_string(static_cast<float>(height) / V_UNIT);
}

void StoneWall::edit(StackMaxCapacity<Action, 1000>& undos)
{
    undos.push(Action(
        [this] (Action::Param& old_width_and_height)
        {
            width = old_width_and_height.four_byte1;
            height = old_width_and_height.four_byte2;

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


}

#endif