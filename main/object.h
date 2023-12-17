#pragma once

#include <fstream>
#include <vector>

#include "collidable.h"
#include "graphics.h"
#include "vec2.h"

class Object
{
public:
    Object(const std::vector<Collidable>& collidables, const std::vector<Collidable>& killables);

    virtual void move(const Vec2I& amount);
    virtual void draw(Graphics& g) const;

    inline const std::vector<Collidable>& get_collidables() const { return collidables; }
    inline const std::vector<Collidable>& get_killables() const { return killables; }

    constexpr static int WOODEN_FLOOR = 0, SPIKE = 1, INVISIBLE_BOUNDRY = 2;

    virtual void write_to_file(std::ofstream& output_file) const = 0;

protected:
    std::vector<Collidable> collidables;
    std::vector<Collidable> killables;
};