#pragma once

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

protected:
    std::vector<Collidable> collidables;
    std::vector<Collidable> killables;
};