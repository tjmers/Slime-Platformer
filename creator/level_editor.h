#pragma once

#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR
#endif

#include <deque>
#include <iterator>
#include <string>
#include <vector>

#include "action.h"
#include "stack_max_capacity.h"
#include "../main/level_loader.h"
#include "../main/io_assistance.h"
#include "../main/object.h"
#include "../main/graphics.h"
#include "../main/vec2.h"
#include "../objects/decoy.h"
#include "../objects/invisible_boundry.h"
#include "../objects/spike.h"
#include "../objects/wooden_floor.h"


/// @brief Make a level. When saving a level, the save file contains the data in VUNITS and HUNITS, they get converted when the program runs
class LevelEditor
{
public:
    LevelEditor(); // used to make a new one
    LevelEditor(const std::string& file_path); // used to load from a file
    LevelEditor(Vec2F player_position, float player_left_bound, float player_right_bound, float player_upper_bound, float player_lower_bound, std::vector<Object*>& objects);
    ~LevelEditor();
    static HRESULT init_resources(Graphics& g);

    void draw(Graphics& g) const;
    void update();

private:
    std::string file_path;
    Vec2F player_position;
    D2D1_RECT_F player_bounds;
    bool draw_player_bounds;
    std::vector<Object*> objects; // the first will be used for the player sprite
    static std::string get_file_path();

    constexpr static int PAN_SPEED = 4;
    constexpr static int OBJECT_MOVE_SPEED = 1;
    constexpr static int SPEED_MULTIPLIER = 5;
    Vec2I total_panned;
    bool selected;
    std::vector<Object*>::iterator object_selected;
    void update_selection();
    void update_pan();
    void move_selected_object();
    void check_front_ordering();
    void check_back_ordering();
    void create_object();
    /// @brief Uses the terminal to edit the object properties
    void edit_object_properties_from_menu();
    void edit_level_properties();

    bool object_menu_open;
    Object::TYPE object_selected_from_menu;

    static_assert(sizeof(long long) == sizeof(void*), "long long size is not the same as void* size");
    static_assert(sizeof(long long) == sizeof(int) * 2, "long long size is not twice the size of an int");
    static_assert(sizeof(Object*) == sizeof(std::vector<Object*>::iterator), "size of object vector iterator is not the same as the Object*");
    StackMaxCapacity<Action, 100> undos;
    void undo();
    void add_selected_object_change_to_undos();

    std::deque<Object*> deleted_objects;

    // object properties
    int wooden_floor_width, wooden_floor_height;
    Spike::Facing spike_rotation;
    Side invisible_boundry_side;
    int invisible_boundry_length;

    // object sprites in the object menu
    static ID2D1Bitmap* object_menu_sprites[static_cast<int>(Object::TYPE::COUNT)];
    static D2D1_RECT_F object_menu_sprite_bounds[static_cast<int>(Object::TYPE::COUNT)];


};