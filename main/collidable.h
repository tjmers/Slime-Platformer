#pragma once

#include "graphics.h"
#include "vec2.h"

enum class Side
{
    LEFT, RIGHT, TOP, BOTTOM
};

class Collidable
{

public:

    Collidable(Side side, int x, int y, int length);
    // bool collides(const Player& p) const; // move this to the player class

    inline int get_x() const { return x; }
    inline int get_y() const { return y; }
    inline int get_length() const { return length; }
    inline Side get_side() const { return side; }

    void move(int x, int y);
    void move(const Vec2I& amount);

#ifdef LEVEL_EDITOR
    inline void set_length(int new_length) { length = new_length; }
    inline void set_side(Side new_side) { side = new_side; }

#endif

#ifdef DRAW_HITBOXES
    void draw(Graphics& g) const;
#endif
private:

#ifndef LEVEL_EDITOR
    const
#endif
    Side side;
    int x, y;

#ifndef LEVEL_EDITOR
    const
#endif
    int length;
};