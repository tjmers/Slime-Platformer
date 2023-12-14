#include "level.h"

#include <stdexcept>

#include "input.h"


Level::Level(int id)
    : player(get_player(id)), objects(get_objects(id)), distance_moved(0, 0)
{}

Level::~Level()
{
    for (Object* o : objects)
        delete o;
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

std::vector<Object*> Level::get_objects(int level_id)
{
    switch (level_id)
    {
    case 0:
        return { new WoodenFloor(0, SCREEN_WIDTH, 28_vu, 4_vu) };
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
    Vec2I amount_to_shift = player.update(multiplier, objects);

    if (amount_to_shift)
    {
        for (Object* o : objects)
        {
            o->move(amount_to_shift);
        }
        distance_moved += amount_to_shift;
    }
}

void Level::draw(Graphics& g) const
{
    for (const Object* o : objects)
        o->draw(g);
    g.SetColor(D2D1::ColorF::CornflowerBlue);
    player.draw(g);
}

void Level::reset()
{
    distance_moved *= -1;
    player.reset();
    for (Object* o : objects)
        o->move(distance_moved);

    distance_moved.x = 0;
    distance_moved.y = 0;
}



HRESULT Level::init_resources(Graphics& g)
{
    HRESULT hr = Player::init(g);
    if (SUCCEEDED(hr))
        hr = WoodenFloor::init(g);

    return hr;
}