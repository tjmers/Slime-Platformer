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

#ifdef DRAW_COLLIDABLES_
    void draw(Graphics& g) const;
#endif // DRAW_COLLIDABLES_
private:
    const Side side;
    int x, y, length;
};