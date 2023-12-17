#include "level_editor.h"

#include <fstream>
#include <iostream>

#include "../objects/invisible_boundry.h"
#include "../objects/spike.h"
#include "../objects/wooden_floor.h"
#include "../main/input.h"


LevelEditor::LevelEditor()
    : file_path(get_file_path()), player_position(0.0f, 0.0f), objects(),
      player_left_bound(0), player_right_bound(0), player_upper_bound(0), player_lower_bound(0),
      total_panned(0, 0)
{

}

LevelEditor::LevelEditor(const std::string& file_path)
    : file_path(std::move(file_path)), player_position(get_player_position(this->file_path)), objects(get_objects(this->file_path)),
      total_panned(0, 0)
{
    get_player_bounds();
}

LevelEditor::LevelEditor(Vec2F player_position, int player_left_bound, int player_right_bound, int player_upper_bound, int player_lower_bound, std::vector<Object*>& objects)
    : player_position(player_position), player_left_bound(player_left_bound), player_right_bound(player_right_bound), player_upper_bound(player_upper_bound), player_lower_bound(player_lower_bound),
      objects(objects), file_path(get_file_path()) {}

std::string LevelEditor::get_file_path()
{
    std::cout << "Enter file path for the new file to be saved to";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

Vec2F LevelEditor::get_player_position(const std::string& file_path)
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

std::vector<Object*> LevelEditor::get_objects(const std::string& file_path)
{
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
            switch (std::stoi(line))
            {
            case Object::WOODEN_FLOOR:
                interpret_wooden_floor(line, input_file, objects);
                break;
            case Object::SPIKE:
                interpret_spike(line, input_file, objects);
                break;
            case Object::INVISIBLE_BOUNDRY:
                interpret_invisible_boundry(line, input_file, objects);
                break;
            }
        }

        input_file.close();
    }

    return objects;
}

void LevelEditor::interpret_wooden_floor(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
{
    std::getline(input_file, line);
    int x1 = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int x2 = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int y = static_cast<int>(std::stof(line) * V_UNIT);
    std::getline(input_file, line);
    int height = static_cast<int>(std::stof(line) * V_UNIT);
    objects.push_back(new WoodenFloor(x1, x2, y, height));
}

void LevelEditor::interpret_spike(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
{
    std::getline(input_file, line);
    Spike::Facing direction = static_cast<Spike::Facing>(std::stoi(line));
    std::getline(input_file, line);
    int x = static_cast<int>(std::stof(line) * H_UNIT);
    std::getline(input_file, line);
    int y = static_cast<int>(std::stof(line) * V_UNIT);
    objects.push_back(new Spike(direction, x, y));
}

void LevelEditor::interpret_invisible_boundry(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
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

void LevelEditor::get_player_bounds()
{
    std::ifstream input_file(file_path);
    std::string line;
    if (input_file.is_open())
    {
        std::getline(input_file, line);
        std::getline(input_file, line);
        std::getline(input_file, line);
        player_left_bound = static_cast<int>(std::stof(line) / H_UNIT);
        std::getline(input_file, line);
        player_right_bound = static_cast<int>(std::stof(line) / H_UNIT);
        std::getline(input_file, line);
        player_upper_bound = static_cast<int>(std::stof(line) / V_UNIT);
        std::getline(input_file, line);
        player_lower_bound = static_cast<int>(std::stof(line) / V_UNIT);
        input_file.close();
    }
}

void LevelEditor::save() const
{
    std::ofstream output_file(file_path);

    if (output_file.is_open())
    {
        // write the player data
        output_file << std::to_string(static_cast<float>(player_position.x) / H_UNIT) << '\n' << std::to_string(static_cast<float>(player_position.y) / V_UNIT) << '\n'
                    << std::to_string(static_cast<float>(player_left_bound) / H_UNIT) << '\n' << std::to_string(static_cast<float>(player_right_bound) / H_UNIT) << '\n'
                    << std::to_string(static_cast<float>(player_upper_bound) / V_UNIT) << '\n' << std::to_string(static_cast<float>(player_lower_bound) / H_UNIT);

        for (Object* o : objects)
            o->write_to_file(output_file);

        output_file.close();
    }
    else
    {
        MessageBoxA(NULL, "Something went wrong", "An error occured when opening the file to write", MB_OK);
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }
}


LevelEditor::~LevelEditor()
{
    for (Object* o : objects)
        delete o;
}


void LevelEditor::update()
{
    update_selection();

    update_pan();


}

void LevelEditor::update_selection()
{
    std::cout << Input::GetLeftMouseDown() << ' ' << Input::GetLeftMousePosition().x << ' ' << Input::GetLeftMousePosition().y << '\n';
}

void LevelEditor::update_pan()
{
    Vec2I amount_to_move(0, 0);

    // amount_to_move.x += PAN_SPEED * (Input::GetKeyDown(Key::D) - Input::GetKeyDown(Key::A)); // should do same thing but idk

    if (Input::GetKeyDown(Key::A))
        amount_to_move.x -= PAN_SPEED;

    if (Input::GetKeyDown(Key::D))
        amount_to_move.x += PAN_SPEED;

    if (Input::GetKeyDown(Key::W))
        amount_to_move.y -= PAN_SPEED;
    
    if (Input::GetKeyDown(Key::S))
        amount_to_move.y += PAN_SPEED;

    for (Object* o : objects)
        o->move(amount_to_move);

    total_panned += amount_to_move;
}

void LevelEditor::draw(Graphics& g) const
{ // TODO
    for (Object* o : objects)
        o->draw(g);
}

HRESULT LevelEditor::init_resources(Graphics& g)
{
    HRESULT hr = WoodenFloor::init(g);

    if (SUCCEEDED(hr))
        hr = Spike::init(g);

    return hr;
}