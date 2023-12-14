#pragma once

#include <stdexcept>
#include <vector>

#include "collidable.h"
#include "graphics.h"
#include "object.h"
#include "vec2.h"


class Spike : public Object
{

public:
    enum class Facing
    {
        LEFT, RIGHT, UP, DOWN
    };
    Spike(Facing direction, int x, int y);
    static HRESULT init(Graphics& g);
    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;
private:
    Vec2I position;
    constexpr static int WIDTH = 1_hu, HEIGHT = 1_vu;
    static std::vector<Collidable> make_killables(Facing direction, int x, int y);
    static ID2D1Bitmap* sprites;
    D2D1_RECT_F sprite_box;
};