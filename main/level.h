#pragma once

#include <vector>

#include "graphics.h"
#include "object.h"
#include "player.h"

class Level
{
public:

    Level(int id);
    Level(const Player& player, const std::vector<Object*> objects);
    ~Level();

    Level() = delete;

    void update(const float multiplier);
    void draw(Graphics& g) const;
    void reset();
    static HRESULT init_resources(Graphics& g);

private:
    static Player get_player(int level_id);
    static std::vector<Object*> get_objects(int level_id);
    Player player;
    std::vector<Object*> objects;
    Vec2I distance_moved;
};