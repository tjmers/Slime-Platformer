#pragma once

#include "../main/object.h"


class Stone : public Object
{

public:
    Stone(const Vec2I& position, const int width, const int height);

    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;


#ifdef LEVEL_EDITOR
    Stone(const Stone& other);
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
    Vec2I position;
#ifndef LEVEL_EDITOR
    const
#endif
    int width, height;

    inline static std::vector<Collidable> make_collidables(int x, int y, int width, int height)
    {
        return { {Side::BOTTOM, x, y, width}, {Side::LEFT, x + width, y, height}, {Side::TOP, x, y + height, width}, {Side::RIGHT, x, y, height} };
    }

    static ID2D1Bitmap* sprite;
};