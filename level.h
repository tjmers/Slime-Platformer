#pragma once

#include <vector>

#ifndef DRAW_COLLIDABLES_
#define DRAW_COLLIDABLES_
#endif
#include "graphics.h"
#include "player.h"

class Level
{
public:

    Level(int id);
    ~Level();

    Level() = delete;

    void update(const float multiplier);
    void draw(Graphics& g) const;

private:
    static Player get_player(int level_id);
    static std::vector<Collidable> get_collidables(int level_id);
    Player player;
    std::vector<Collidable> collidables;
};