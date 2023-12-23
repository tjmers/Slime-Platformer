#include "level_loader.h"

#include <iostream>
#include <stdexcept>

#include "../objects/decoy.h"
#include "../objects/invisible_boundry.h"
#include "../objects/spike.h"
#include "../objects/wooden_floor.h"

namespace LevelLoader
{

Vec2F get_player_position(const std::string& file_path)
{
    Vec2F position(0.0f, 0.0f);
    std::ifstream input_file(file_path);

    if (input_file.is_open())
    {
        std::string line;
        std::getline(input_file, line);
        position.x = std::stof(line) * H_UNIT;
        std::getline(input_file, line);
        position.y = std::stof(line) * V_UNIT;
        input_file.close();
    }
    else
    {
        throw std::invalid_argument("invalid file path");
    }
    return position;
}


D2D1_RECT_F get_player_bounds(const std::string& file_path)
{
    std::ifstream input_file(file_path);
    std::string line;
    D2D1_RECT_F ret_val;
    if (input_file.is_open())
    {
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        ret_val.left = static_cast<int>(std::stof(line) * H_UNIT);
        std::getline(input_file, line);
        ret_val.right = static_cast<int>(std::stof(line) * H_UNIT);
        std::getline(input_file, line);
        ret_val.top = static_cast<int>(std::stof(line) * V_UNIT);
        std::getline(input_file, line);
        ret_val.bottom = static_cast<int>(std::stof(line) * V_UNIT);
        input_file.close();
    }
    else
        throw std::invalid_argument("invalid file_path passed to LevelLoader::get_player_bounds(std::string&)");

    return ret_val;
}


std::vector<Object*> get_objects(const std::string& file_path, const Vec2I& player_position)
{
    std::vector<Object*> objects;
#ifdef LEVEL_EDITOR
    objects.push_back(new Decoy(player_position));
#endif
    std::ifstream input_file(file_path);
    
    if (input_file.is_open())
    {
        std::string line;
        // skip the first six lines; they are used for player data
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        while (std::getline(input_file, line))
        {
            switch (static_cast<Object::TYPE>(std::stoi(line)))
            {
            case Object::TYPE::WOODEN_FLOOR:
                interpret_wooden_floor(line, input_file, objects);
                break;
            case Object::TYPE::SPIKE:
                interpret_spike(line, input_file, objects);
                break;
            case Object::TYPE::INVISIBLE_BOUNDRY:
                interpret_invisible_boundry(line, input_file, objects);
                break;
            case Object::TYPE::DECOY:
                throw std::invalid_argument("cannot interpret a decoy");

            default:
                throw std::invalid_argument("error while parsing file by loading object with id: " + line);
            }
        }

        input_file.close();
    }

    return objects;
}

std::vector<Object*> get_objects(const std::string& file_path)
{

#ifdef LEVEL_EDITOR
    return get_objects(file_path, get_player_position(file_path));
#else
    std::vector<Object*> objects;
    std::ifstream input_file(file_path);
    
    if (input_file.is_open())
    {
        std::string line;
        // skip the first six lines; they are used for player data
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        while (std::getline(input_file, line))
        {
            switch (static_cast<Object::TYPE>(std::stoi(line)))
            {
            case Object::TYPE::WOODEN_FLOOR:
                interpret_wooden_floor(line, input_file, objects);
                break;
            case Object::TYPE::SPIKE:
                interpret_spike(line, input_file, objects);
                break;
            case Object::TYPE::INVISIBLE_BOUNDRY:
                interpret_invisible_boundry(line, input_file, objects);
                break;
            case Object::TYPE::DECOY:
                throw std::invalid_argument("cannot interpret a decoy");

            default:
                throw std::invalid_argument("error while parsing file by loading object with id: " + line);
            }
        }

        input_file.close();
    }

    return objects;

#endif
}


void interpret_wooden_floor(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
{
    std::getline(input_file, line);
    int x = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int y = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int width = static_cast<int>(std::stof(line) * V_UNIT);
    std::getline(input_file, line);
    int height = static_cast<int>(std::stof(line) * V_UNIT);
    objects.push_back(new WoodenFloor(x, y, width, height));
}


void interpret_spike(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
{
    std::getline(input_file, line);
    Spike::Facing direction = static_cast<Spike::Facing>(std::stoi(line));
    std::getline(input_file, line);
    int x = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int y = static_cast<int>(std::stof(line) * V_UNIT);
    objects.push_back(new Spike(direction, x, y));
}

void interpret_invisible_boundry(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
{
    std::getline(input_file, line);
    Side side = static_cast<Side>(std::stoi(line));
    std::getline(input_file, line);
    int x = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int y = static_cast<int>(std::stof(line) * V_UNIT);
    std::getline(input_file, line);
    int length;
    if (side == Side::TOP || side == Side::BOTTOM)
        length = static_cast<int>(std::stof(line) * H_UNIT);
    else
        length = static_cast<int>(std::stof(line) * V_UNIT);

    objects.push_back(new InvisibleBoundry(side, x, y, length));
}

#ifdef LEVEL_EDITOR
void save_objects(std::string& file_path, const std::vector<Object*>& objects, const Vec2I& amount_moved, const Vec2I& player_position, const D2D1_RECT_F& player_bounds)
{
    // move back the objects
    Vec2I amount_to_move_objects = amount_moved * -1;
    for (Object* o : objects)
        o->move(amount_to_move_objects);

    std::ofstream output_file(file_path);

    if (output_file.is_open())
    {
        // write the player data
        output_file << std::to_string(static_cast<float>(player_position.x) / H_UNIT) << '\n' << std::to_string(static_cast<float>(player_position.y) / V_UNIT) << '\n'
                    << std::to_string(static_cast<float>(player_bounds.left) / H_UNIT) << '\n' << std::to_string(static_cast<float>(player_bounds.right) / H_UNIT) << '\n'
                    << std::to_string(static_cast<float>(player_bounds.top) / V_UNIT) << '\n' << std::to_string(static_cast<float>(player_bounds.bottom) / H_UNIT);

        for (auto i = std::next(objects.begin()); i != objects.end(); ++i)
            (*i)->write_to_file(output_file); // we dont want to write the decoy to the save file
        

        std::cout << "Level successfully saved\n";
        output_file.close();
    }
    else
    {
        MessageBoxA(NULL, "Something went wrong", "An error occured when opening the file to write", MB_OK);
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }

    // move back
    for (Object* o : objects)
        o->move(amount_moved);
}
#endif

}


