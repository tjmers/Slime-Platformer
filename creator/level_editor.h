#pragma once

#include <iterator>
#include <string>
#include <vector>

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
    LevelEditor(Vec2F player_position, int player_left_bound, int player_right_bound, int player_upper_bound, int player_lower_bound, std::vector<Object*>& objects);
    ~LevelEditor();
    static HRESULT init_resources(Graphics& g);

    void draw(Graphics& g) const;
    void update();
    void save() const;

private:
    std::string file_path;
    Vec2F player_position;
    int player_left_bound, player_right_bound, player_upper_bound, player_lower_bound;
    std::vector<Object*> objects; // the first will be used for the player sprite
    static std::string get_file_path();
    static Vec2F get_player_position(const std::string& file_path);
    static std::vector<Object*> get_objects(const std::string& file_path, const Vec2I& player_position);
    void get_player_bounds();
    static void interpret_wooden_floor(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);
    static void interpret_spike(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);
    static void interpret_invisible_boundry(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);
    static void interpret_decoy(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);

    constexpr static int PAN_SPEED = 4;
    constexpr static int OBJECT_MOVE_SPEED = 1;
    constexpr static int SPEED_MULTIPLIER = 5;
    Vec2I total_panned;
    bool selected;
    std::vector<Object*>::iterator object_selected;
    void update_selection();
    void update_pan();
    void move_selected_object();
    void create_object();
    /// @brief Uses the terminal to edit the object properties
    void edit_object_properties_from_menu();

    bool object_menu_open;
    Object::TYPE object_selected_from_menu;

    static int get_valid_int(const std::string& error_message, std::string& line);
    static int get_valid_int(const std::string& error_message, std::string& line, int lower_bound, int upper_bound); // lower - included, upper - excluded

    // object properties
    int wooden_floor_width, wooden_floor_height;
    Spike::Facing spike_rotation;
    Side invisible_boundry_side;
    int invisible_boundry_length;

    // object sprites in the object menu
    static ID2D1Bitmap* object_menu_sprites[static_cast<int>(Object::TYPE::COUNT)];
    static D2D1_RECT_F object_menu_sprite_bounds[static_cast<int>(Object::TYPE::COUNT)];
};