#pragma once

#include "../main/object.h"


class Grass : public Object
{
public:
    Grass(const Vec2I& position, const int width, const int height);

    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;

#ifdef LEVEL_EDITOR
    Grass(const Grass& other);
    Object* clone() const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
    void write_to_file(std::ofstream& output_file) const override;
    void edit(StackMaxCapacity<Action, 1000>& undos) override;
#endif

    static HRESULT init(Graphics& g);
    constexpr static int MAX_HEIGHT = 2_vu;

private:
    inline static std::vector<Collidable> make_collidables(const Vec2I& position, const int width, const int height)
    {
        return { Collidable(Side::BOTTOM, position.x, position.y, width), Collidable(Side::RIGHT, position.x, position.y, height), Collidable(Side::LEFT, position.x + width, position.y, height)};
    }
    Vec2I position;

#ifndef LEVEL_EDITOR
    const
#endif
    int width, height;

    static ID2D1Bitmap* sprite;
};