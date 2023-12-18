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
    virtual int get_x() const = 0; // ------\.
    virtual int get_y() const = 0; //       These just are used for the level editor to determine
    virtual int get_width() const = 0;//    /               if an click intersects an object to select it
    virtual int get_height() const = 0;//--/                    (maybe wrap it in a #ifdef later)

    constexpr static int WOODEN_FLOOR = 0, SPIKE = 1, INVISIBLE_BOUNDRY = 2, DECOY = 3;

    virtual void write_to_file(std::ofstream& output_file) const = 0;

protected:
    std::vector<Collidable> collidables;
    std::vector<Collidable> killables;
};