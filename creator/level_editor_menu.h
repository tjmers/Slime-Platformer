#pragma once

#include "../main/graphics.h"
#include "../main/object.h"
#include "../objects/invisible_boundry.h"
#include "../objects/stone.h"
#include "../objects/spike.h"
#include "../objects/wooden_floor.h"


class LevelEditorMenu
{
public:
    LevelEditorMenu();

    Object* get_object(const Vec2I& position) const;
    bool update();
    void edit();


    inline bool get_active() const { return active; }
    inline void set_active(bool b) { active = b; }

    static HRESULT init(Graphics& g);

    void draw(Graphics& g) const;

private:

    bool active;

    Object::TYPE selected_object;

    // object properties
    int wooden_floor_width, wooden_floor_height;
    Spike::Facing spike_rotation;
    Side invisible_boundry_side;
    int invisible_boundry_length;
    int stone_width, stone_height;

    // object sprites in the object menu
    static ID2D1Bitmap* sprites[static_cast<int>(Object::TYPE::COUNT)];
    static D2D1_RECT_F sprite_bounds[static_cast<int>(Object::TYPE::COUNT)];
};