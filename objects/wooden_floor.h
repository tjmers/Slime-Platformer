#pragma once

#include <vector>

#include "../main/object.h"
#include "../creator/stack_max_capacity.h"
#include "../creator/action.h"


class WoodenFloor : public Object
{
public:
    WoodenFloor(const int& x, const int& y, const int& width, const int& height);
    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;
    static HRESULT init(Graphics& g);

#ifdef LEVEL_EDITOR
    WoodenFloor(const WoodenFloor& other);
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
    
    #ifndef LEVEL_EDITOR
    const
    #endif
    int width, height;

    static std::vector<Collidable> make_collidables(const int& x1, const int& width, const int& y);
    static ID2D1Bitmap* sprite;
    constexpr static int SPRITE_WIDTH = 128, SPRITE_HEIGHT = 32;
};