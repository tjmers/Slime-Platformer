#include "object.h"

Object::Object(const std::vector<Collidable>& collidables, const std::vector<Collidable>& killables)
    : collidables(collidables), killables(killables) {}


void Object::move(const Vec2I& amount)
{
    for (Collidable& c : collidables)
        c.move(amount);

    for (Collidable& c : killables)
        c.move(amount);

}

void Object::draw(Graphics& g) const
{
#ifdef DRAW_HITBOXES
    g.SetColor(D2D1::ColorF::Blue);
    for (const Collidable& c : collidables)
        c.draw(g);

    g.SetColor(D2D1::ColorF::Red);

    for (const Collidable& c : killables)
        c.draw(g);
#endif
}