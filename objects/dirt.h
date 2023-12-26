#pragma once

#include "../main/object.h"


class Dirt : public Object
{
public:
    Dirt(const Vec2I& position, const int width, const int height);

    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;

#ifdef LEVEL_EDITOR
    Dirt(const Dirt& other);
    Object* clone() const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
    void write_to_file(std::ofstream& output_file) const override;
    void edit(StackMaxCapacity<Action, 1000>& undos) override;
#endif

    static HRESULT init(Graphics& g);

private:
    inline static std::vector<Collidable> make_collidables(const Vec2I& position, const int width, const int height)
    {
        return { Collidable(Side::BOTTOM, position.x, position.y, width), Collidable(Side::RIGHT, position.x, position.y, height), Collidable(Side::LEFT, position.x + width, position.y, height), Collidable(Side::TOP, position.x, position.y + height, width) };
    }
    Vec2I position;

#ifndef LEVEL_EDITOR
    const
#endif
    int width, height;

    static ID2D1Bitmap* sprite;
    constexpr static int SPRITE_WIDTH = 32, SPRITE_HEIGHT = 32;
};