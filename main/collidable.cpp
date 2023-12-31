#include "collidable.h"

Collidable::Collidable(Side side, int x, int y, int length)
    : side(side), x(x), y(y), length(length) {}

#ifdef DRAW_HITBOXES
void Collidable::draw(Graphics& g) const
{
    if (side == Side::TOP || side == Side::BOTTOM)
        g.DrawLine(D2D1::Point2F((float)x, (float)y), D2D1::Point2F((float)(x + length), (float)y), 2.0f);
    else
        g.DrawLine(D2D1::Point2F((float)x, (float)y), D2D1::Point2F((float)x, (float)(y + length)), 2.0f);
    
}
#endif


void Collidable::move(int x, int y)
{
    this->x += x;
    this->y += y;
}

void Collidable::move(const Vec2I& amount)
{
    x += amount.x;
    y += amount.y;
}