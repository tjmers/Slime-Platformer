#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "graphics.h"
#include "object.h"
#include "vec2.h"


namespace LevelLoader
{

Vec2F get_player_position(const std::string& file_path);
D2D1_RECT_F get_player_bounds(const std::string& file_path);

std::vector<Object*> get_objects(const std::string& file_path, const Vec2I& player_position);
std::vector<Object*> get_objects(const std::string& file_path);
void interpret_wooden_floor(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);
void interpret_spike(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);
void interpret_invisible_boundry(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects);

#ifdef LEVEL_EDITOR
void save_objects(std::string& file_path, const std::vector<Object*>& objects, const Vec2I& amount_moved, const Vec2I& player_position, const D2D1_RECT_F& player_bounds);
#endif
}