#include "level_editor_menu.h"

#include <string>
#include <iostream>

#include "../main/input.h"
#include "../main/io_assistance.h"


LevelEditorMenu::LevelEditorMenu()
    : selected_object(static_cast<Object::TYPE>(0)), active(false),
      wooden_floor_width(100), wooden_floor_height(100),
      spike_rotation(Spike::Facing::UP),
      invisible_boundry_side(Side::LEFT), invisible_boundry_length(50),
      stone_width(H_UNIT), stone_height(V_UNIT),
      dirt_width(2_vu), dirt_height(2_vu),
      grass_width(8_vu), grass_height(Grass::MAX_HEIGHT),
      stone_wall_width(2_hu), stone_wall_height(2_vu) {}

void LevelEditorMenu::draw(Graphics& g) const
{
    g.SetColor(D2D1::ColorF::AntiqueWhite);
    g.FillRect(D2D1::RectF(36_hu, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    g.DrawBitmap(sprites[static_cast<int>(selected_object)], sprite_bounds[static_cast<int>(selected_object)], { 39.0_hu, 13.0_vu, 45.0_hu, 19.0_vu});
    g.DrawBitmap(sprites[(static_cast<int>(selected_object) + 1) % static_cast<int>(Object::TYPE::COUNT)], sprite_bounds[(static_cast<int>(selected_object) + 1) % static_cast<int>(Object::TYPE::COUNT)], { 40.0_hu, 23.0_vu, 45.0_hu, 28.0_vu});
    int next_object = static_cast<int>(selected_object) - 1;
    if (next_object < 0)
        next_object = static_cast<int>(Object::TYPE::COUNT) - 1;

    g.DrawBitmap(sprites[next_object], sprite_bounds[next_object], { 40.0_hu, 5_vu, 45.0_hu, 9.0_vu });

    g.SetColor(D2D1::ColorF::Black);
    g.DrawRect(D2D1::RectF(39.0_hu, 13.0_vu, 45.0_hu, 19.0_vu), 3.0f);
}


bool LevelEditorMenu::update()
{
    bool should_create_object = false;
    if (Input::GetKeyDown(Key::E) && Input::GetKeyFrame(Key::E) == 0)
            edit();
        
    if (Input::GetLeftMouseDown() && Input::GetLeftMouseFrame() == 0)
        should_create_object = true;

    if (Input::GetKeyDown(Key::DOWN) && Input::GetKeyFrame(Key::DOWN) == 0)
        selected_object = static_cast<Object::TYPE>((static_cast<int>(selected_object) + 1) % static_cast<int>(Object::TYPE::COUNT));

    if (Input::GetKeyDown(Key::UP) && Input::GetKeyFrame(Key::UP) == 0)
    {
        int decremented_object = static_cast<int>(selected_object) - 1;
        if (decremented_object == -1)
            selected_object = static_cast<Object::TYPE>(static_cast<int>(Object::TYPE::COUNT) - 1);
        
        else
            selected_object = static_cast<Object::TYPE>(decremented_object);
    }

    return should_create_object;
}

Object* LevelEditorMenu::get_object(const Vec2I& position) const
{
    switch (selected_object)
    {
    case Object::TYPE::WOODEN_FLOOR:
        return new WoodenFloor(position.x, position.y, wooden_floor_width, wooden_floor_height);

    case Object::TYPE::SPIKE:
        return new Spike(spike_rotation, position.x, position.y);
        
    case Object::TYPE::INVISIBLE_BOUNDRY:
        return new InvisibleBoundry(invisible_boundry_side, position.x, position.y, invisible_boundry_length);
        
    case Object::TYPE::STONE:
        return new Stone(position, stone_width, stone_height);
    
    case Object::TYPE::GRASS:
        return new Grass(position, grass_width, grass_height);
    
    case Object::TYPE::DIRT:
        return new Dirt(position, dirt_width, dirt_height);

    case Object::TYPE::STONE_WALL:
        return new StoneWall(position, stone_wall_width, stone_wall_height);

    default:
        throw std::invalid_argument("trying to create object with an invalid ID");
    }
}

ID2D1Bitmap* LevelEditorMenu::sprites[static_cast<int>(Object::TYPE::COUNT)];
D2D1_RECT_F LevelEditorMenu::sprite_bounds[static_cast<int>(Object::TYPE::COUNT)];

void LevelEditorMenu::edit()
{
    std::string line;
    std::cout << "Enter a dash '-' to keep the same value\n";
    switch (selected_object)
    {
    case Object::TYPE::WOODEN_FLOOR:
        std::cout << "Width (" << static_cast<float>(wooden_floor_width / H_UNIT) << "): ";
        std::cin >> line;
        
        if (line != "-")
            wooden_floor_width = static_cast<int>(IoAssistance::get_valid_float("Invalid width -- try again: ", line) * H_UNIT);
        

        std::cout << "Height (" << static_cast<float>(wooden_floor_height / V_UNIT) << "): ";
        std::cin >> line;

        if (line != "-")
            wooden_floor_height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- try again: ", line) * H_UNIT);

        break;
    case Object::TYPE::SPIKE:
        std::cout << "Enter rotation: left [0], right [1], up [2], down [3]: ";
        std::cin >> line;

        if (line != "-")
            spike_rotation = static_cast<Spike::Facing>(IoAssistance::get_valid_int("Rotation must be a number: left [0], right [1], up [2], down [3]: ", line, 0, 4));

        break;
    case Object::TYPE::INVISIBLE_BOUNDRY:
        std::cout << "Enter side: left [0], right [1], top [2], bottom [3]: ";
        std::cin >> line;

        if (line != "-")
            invisible_boundry_side = static_cast<Side>(IoAssistance::get_valid_int("Side must be a number: left [0], right [1], top [2], bottom [3]: ", line, 0, 4));

        std::cout << "Enter length for side (" << invisible_boundry_length << "): ";
        std::cin >> line;
        if (line != "-")
            invisible_boundry_length = IoAssistance::get_valid_int("Length must be a valid number: ", line);

        break;
    case Object::TYPE::STONE:
        std::cout << "Width (" << static_cast<float>(stone_width / H_UNIT) << "): ";
        std::cin >> line;
        
        if (line != "-")
            stone_width = static_cast<int>(IoAssistance::get_valid_float("Invalid width -- try again: ", line) * H_UNIT);
        

        std::cout << "Height (" << static_cast<float>(stone_height / V_UNIT) << "): ";
        std::cin >> line;

        if (line != "-")
            stone_height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- try again: ", line) * H_UNIT);

        break;
    case Object::TYPE::GRASS:
        std::cout << "Width (" << static_cast<float>(grass_width / H_UNIT) << "): ";
        std::cin >> line;
        
        if (line != "-")
            grass_width = static_cast<int>(IoAssistance::get_valid_float("Invalid width -- try again: ", line) * H_UNIT);
        

        std::cout << "Height (" << static_cast<float>(grass_height / V_UNIT) << "): ";
        std::cin >> line;

        if (line != "-")
            grass_height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- must be between 0 and 2 -- try again: ", line, 0, true, Grass::MAX_HEIGHT, true) * H_UNIT);
        break;
    case Object::TYPE::DIRT:
        std::cout << "Width (" << static_cast<float>(dirt_width / H_UNIT) << "): ";
        std::cin >> line;
        
        if (line != "-")
            dirt_width = static_cast<int>(IoAssistance::get_valid_float("Invalid width -- try again: ", line) * H_UNIT);
        

        std::cout << "Height (" << static_cast<float>(dirt_height / V_UNIT) << "): ";
        std::cin >> line;

        if (line != "-")
            dirt_height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- try again: ", line) * H_UNIT);
        break;

    case Object::TYPE::STONE_WALL:
        std::cout << "Width (" << static_cast<float>(stone_wall_width / H_UNIT) << "): ";
        std::cin >> line;
        
        if (line != "-")
            stone_wall_width = static_cast<int>(IoAssistance::get_valid_float("Invalid width -- try again: ", line) * H_UNIT);
        

        std::cout << "Height (" << static_cast<float>(stone_wall_height / V_UNIT) << "): ";
        std::cin >> line;

        if (line != "-")
            stone_wall_height = static_cast<int>(IoAssistance::get_valid_float("Invalid height -- try again: ", line) * H_UNIT);
        break;
    default:
        throw std::invalid_argument("invalid object id to edit: " + std::to_string(static_cast<int>(selected_object)));
    }
    std::cout << "Object edited\n";
}


HRESULT LevelEditorMenu::init(Graphics& g)
{
    HRESULT hr = 0;
    for (int i = 0; i < static_cast<int>(Object::TYPE::COUNT); ++i)
    {
        if (SUCCEEDED(hr))
        {
            switch (static_cast<Object::TYPE>(i))
            {
            case Object::TYPE::WOODEN_FLOOR:
                hr = g.LoadBitmapFromFile(L".\\images\\wooden_floor.png", &sprites[i]);
                sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            case Object::TYPE::SPIKE:
                hr = g.LoadBitmapFromFile(L".\\images\\spikes.png", &sprites[i]);
                sprite_bounds[i] = { 64.0f, 0.0f, 96.0f, 32.0f };
                break;
            case Object::TYPE::INVISIBLE_BOUNDRY:
                hr = g.LoadBitmapFromFile(L".\\images\\red_line.png", &sprites[i]);
                sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            case Object::TYPE::STONE:
                hr = g.LoadBitmapFromFile(L".\\images\\stone.png", &sprites[i]);
                sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            case Object::TYPE::DIRT:
                hr = g.LoadBitmapFromFile(L".\\images\\dirt.png", &sprites[i]);
                sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            case Object::TYPE::GRASS:
                hr = g.LoadBitmapFromFile(L".\\images\\grass.png", &sprites[i]);
                sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            case Object::TYPE::STONE_WALL:
                hr = g.LoadBitmapFromFile(L".\\images\\stone_wall.png", &sprites[i]);
                sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            default:
                throw std::invalid_argument("trying to initialize a sprite with an invalid type");
            }
        }
    }

    return hr;
}