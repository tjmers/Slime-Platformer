#pragma once

#ifdef LEVEL_EDITOR
#include <fstream>
#include <iostream>
#include <string>

#include "../creator/stack_max_capacity.h"
#include "../creator/action.h"

#endif
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
    enum class TYPE
    {
        DECOY = -1, WOODEN_FLOOR, SPIKE, INVISIBLE_BOUNDRY, STONE, GRASS, DIRT, STONE_WALL, COUNT
    };
#ifdef LEVEL_EDITOR
    virtual Object* clone() const = 0;
    virtual int get_x() const = 0; // ------\.
    virtual int get_y() const = 0; //       These just are used for the level editor to determine
    virtual int get_width() const = 0;//    /               if an click intersects an object to select it
    virtual int get_height() const = 0;//--/                    (maybe wrap it in a #ifdef later)


    virtual void write_to_file(std::ofstream& output_file) const = 0;

    
    virtual void edit(StackMaxCapacity<Action, 1000>& undos) = 0;
#endif

protected:
    std::vector<Collidable> collidables;
    std::vector<Collidable> killables;
};