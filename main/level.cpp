#include "level.h"

#include <stdexcept>

#include "input.h"
#include "level_loader.h"
#include "../objects/spike.h"
#include "../objects/invisible_boundry.h"
#include "../objects/wooden_floor.h"


std::array<std::string, 1> Level::default_levels = { ".\\levels\\level1.slime" };


Level::Level(const std::string& file_path)
    : player(LevelLoader::get_player_position(file_path), LevelLoader::get_player_bounds(file_path)), objects(LevelLoader::get_objects(file_path, player.get_position())), distance_moved(0, 0)
{}

Level::Level(const int id)
    : player(get_player(id)), objects(get_objects(id)), distance_moved(0, 0) {}

Level::Level(const Player& player, const std::vector<Object*> objects)
    : player(player), objects(objects), distance_moved(0, 0) {}

Level::~Level()
{
    for (Object* o : objects)
        delete o;
}

Player Level::get_player(int level_id)
{
    if (level_id < 0 || level_id >= default_levels.size())
        throw std::invalid_argument("Invalid level ID");

    return Player(LevelLoader::get_player_position(default_levels[level_id]), LevelLoader::get_player_bounds(default_levels[level_id]));
}

std::vector<Object*> Level::get_objects(int level_id)
{
    if (level_id < 0 || level_id >= default_levels.size())
        throw std::invalid_argument("Invalid level ID");

    return LevelLoader::get_objects(default_levels[level_id]);
}

void Level::update(const float multiplier)
{
    if (Input::GetKeyDown(Key::SPACE) && Input::GetKeyFrame(Key::SPACE) == 0)
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

    if (player.should_reset(objects))
        reset();
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

    if (SUCCEEDED(hr))
        hr = Spike::init(g);


    return hr;
}