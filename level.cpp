#include "level.h"

#include <stdexcept>

#include "input.h"


Level::Level(int id)
    : player(get_player(id)), collidables(get_collidables(id)), distance_moved(0, 0)
{}

Level::~Level()
{

}

Player Level::get_player(int level_id)
{
    switch (level_id)
    {
    case 0:
        return Player(20_hu, 3_vu, 6_hu, SCREEN_WIDTH - 6_hu, 5_vu, SCREEN_HEIGHT - 5_vu);
    default:
        throw std::invalid_argument("Invalid level ID");
    }
}

std::vector<Collidable> Level::get_collidables(int level_id)
{
    switch (level_id)
    {
    case 0:
        return {Collidable(Side::BOTTOM, -30_hu, 30_vu, 104_vu), Collidable(Side::LEFT, 5_hu, 0, 30_vu), Collidable(Side::RIGHT, 40_hu, 0, 30_vu)};
    default:
        throw std::invalid_argument("Invalid level ID");
    }
}

void Level::update(const float multiplier)
{
    if (Input::GetKeyDown(Key::SPACE) && Input::GetKey(Key::SPACE).frame_number == 0)
    {
        reset();
        return;
    }
    Vec2I amount_to_shift = player.update(multiplier, collidables);

    if (amount_to_shift)
    {
        for (Collidable& c : collidables)
        {
            c.move(amount_to_shift);
        }
        distance_moved += amount_to_shift;
    }
}

void Level::draw(Graphics& g) const
{
    g.SetColor(D2D1::ColorF::CornflowerBlue);
    player.draw(g);
#ifdef DRAW_COLLIDABLES_
    g.SetColor(D2D1::ColorF::DeepPink);
    for (const Collidable& c : collidables)
    {
        c.draw(g);
    }
#endif // DRAW_COLLIDABLES_
}

void Level::reset()
{
    distance_moved *= -1;
    player.reset();
    for (Collidable& c : collidables)
        c.move(distance_moved);

    distance_moved.x = 0;
    distance_moved.y = 0;
}