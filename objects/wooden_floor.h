#pragma once

#include <vector>

#include "../main/object.h"


class WoodenFloor : public Object
{
public:
    WoodenFloor(const int& x1, const int& y, const int& width, const int& height);
    void move(const Vec2I& amount) override;
    void draw(Graphics& g) const override;
    static HRESULT init(Graphics& g);
    void write_to_file(std::ofstream& output_file) const override;

    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
private:
    Vec2I position;
    const int width, height;

    static std::vector<Collidable> make_collidables(const int& x1, const int& x2, const int& y, const int& height);
    static ID2D1Bitmap* sprite;
    constexpr static int sprite_width = 128, sprite_height = 32;
};