#include <stdexcept>

#include "level.h"


Level::Level(int id)
    : player(get_player(id)), collidables(get_collidables(id))
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
        return {Collidable(BOTTOM, -30_hu, 30_vu, 104_vu), Collidable(RIGHT, 5_hu, 0, 30_vu)};
    default:
        throw std::invalid_argument("Invalid level ID");
    }
}

void Level::update(const float multiplier)
{
    Vec2I amount_to_shift = player.update(multiplier, collidables);

    if (amount_to_shift)
    {
        for (Collidable& c : collidables)
        {
            c.move(amount_to_shift);
        }
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