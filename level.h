#pragma once

#include <vector>

#ifndef DRAW_COLLIDABLES_
#define DRAW_COLLIDABLES_
#endif
#include "graphics.h"
#include "object.h"
#include "player.h"
#include "wooden_floor.h"

class Level
{
public:

    Level(int id);
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