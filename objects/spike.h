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

#ifdef LEVEL_EDITOR
    Spike(const Spike& other);
    Object* clone() const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
    void write_to_file(std::ofstream& output_file) const override;
    void edit(StackMaxCapacity<Action, 1000>& undos) override;
#endif
private:
    Vec2I position;
    constexpr static int WIDTH = 2_hu, HEIGHT = 2_vu;
    static std::vector<Collidable> make_killables(Facing direction, int x, int y);
    static ID2D1Bitmap* sprites;
    D2D1_RECT_F sprite_box;
};