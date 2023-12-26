#pragma once

#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR
#endif

#include <deque>
#include <iterator>
#include <string>
#include <vector>

#include "action.h"
#include "level_editor_menu.h"
#include "stack_max_capacity.h"
#include "../main/level_loader.h"
#include "../main/io_assistance.h"
#include "../main/object.h"
#include "../main/graphics.h"
#include "../main/vec2.h"
#include "../objects/decoy.h"
#include "../objects/invisible_boundry.h"
#include "../objects/stone.h"
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
    void edit_level_properties();
    void add_adding_object_to_undos();
    void check_selected_object();

    static_assert(sizeof(long long) == sizeof(void*), "long long size is not the same as void* size");
    static_assert(sizeof(long long) == sizeof(int) * 2, "long long size is not twice the size of an int");
    static_assert(sizeof(Object*) == sizeof(std::vector<Object*>::iterator), "size of object vector iterator is not the same as the Object*");
    StackMaxCapacity<Action, 1000> undos;
    void undo();
    void add_selected_object_change_to_undos();

    LevelEditorMenu level_editor_menu;

    std::deque<Object*> deleted_objects;

};