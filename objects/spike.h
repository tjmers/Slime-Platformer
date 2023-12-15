#pragma once

#include <stdexcept>
#include <vector>

#include "../main/object.h"


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
    constexpr static int WIDTH = 2_hu, HEIGHT = 2_vu;
    static std::vector<Collidable> make_killables(Facing direction, int x, int y);
    static ID2D1Bitmap* sprites;
    D2D1_RECT_F sprite_box;
};