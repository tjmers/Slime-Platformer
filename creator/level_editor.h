#pragma once

#include <string>
#include <vector>

#include "../main/object.h"
#include "../main/graphics.h"
#include "../main/vec2.h"


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
};