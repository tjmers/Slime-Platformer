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
D2D1_COLOR_F get_background_color(const std::string& file_path);

#ifdef LEVEL_EDITOR
void save_objects(std::string& file_path, const std::vector<Object*>& objects, const Vec2I& amount_moved, const Vec2I& player_position, const D2D1_RECT_F& player_bounds, const D2D1_COLOR_F& background_color);
#endif
}