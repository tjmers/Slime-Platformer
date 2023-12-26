#pragma once

#include "../main/object.h"


class Decoy : public Object
{
public:
    Decoy(int x, int y);
    Decoy(const Vec2I& position);

    void draw(Graphics& g) const override;
    void move(const Vec2I& amount) override;
    constexpr static int WIDTH = 2_hu, HEIGHT = 2_hu; // the same as the one in the player class
    static_assert(WIDTH == HEIGHT);
    static HRESULT init(Graphics& g);

#ifdef LEVEL_EDITOR
    Object* clone() const override { return nullptr; }
    void write_to_file(std::ofstream& output_file) const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override;
    int get_height() const override;
    void edit(StackMaxCapacity<Action, 1000>&) override;
#endif
private:
    Vec2I position;

    static ID2D1Bitmap* sprite;
    constexpr static D2D1_RECT_F sprite_rect = { 0.0f, 0.0f, 32.0f, 17.0f };
};