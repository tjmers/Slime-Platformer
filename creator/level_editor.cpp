#include "level_editor.h"

#include <fstream>
#include <iostream>

#include "../main/input.h"



LevelEditor::LevelEditor()
    : file_path(get_file_path()), player_position(0.0f, 0.0f), objects({new Decoy(0, 0)}),
      player_left_bound(0), player_right_bound(0), player_upper_bound(0), player_lower_bound(0),
      total_panned(0, 0), selected(false), object_selected(objects.end()),
      object_menu_open(false), object_selected_from_menu(static_cast<Object::TYPE>(0)),
      wooden_floor_width(100), wooden_floor_height(100),
      spike_rotation(Spike::Facing::UP),
      invisible_boundry_side(Side::LEFT), invisible_boundry_length(0)
{}

LevelEditor::LevelEditor(const std::string& file_path)
    : file_path(std::move(file_path)), player_position(get_player_position(this->file_path)), objects(get_objects(this->file_path, player_position)),
      total_panned(0, 0), selected(false), object_selected(objects.end()),
      object_menu_open(false), object_selected_from_menu(static_cast<Object::TYPE>(0)),
      wooden_floor_width(100), wooden_floor_height(100),
      spike_rotation(Spike::Facing::UP),
      invisible_boundry_side(Side::LEFT)
{
    get_player_bounds();
}

LevelEditor::LevelEditor(Vec2F player_position, int player_left_bound, int player_right_bound, int player_upper_bound, int player_lower_bound, std::vector<Object*>& objects)
    : player_position(player_position), player_left_bound(player_left_bound), player_right_bound(player_right_bound), player_upper_bound(player_upper_bound), player_lower_bound(player_lower_bound),
      objects(objects), file_path(get_file_path()), total_panned(0, 0), selected(false), object_selected(objects.end()), object_menu_open(false), object_selected_from_menu(static_cast<Object::TYPE>(0)),
      wooden_floor_width(100), wooden_floor_height(100),
      spike_rotation(Spike::Facing::UP),
      invisible_boundry_side(Side::LEFT), invisible_boundry_length(0)
{}

std::string LevelEditor::get_file_path()
{
    std::cout << "Enter file path for the new file to be saved to: ";
    std::string input;
    std::cin >> input;
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

std::vector<Object*> LevelEditor::get_objects(const std::string& file_path, const Vec2I& player_position)
{
    std::vector<Object*> objects;
    objects.push_back(new Decoy(player_position));
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
                interpret_decoy(line, input_file, objects);
                break;
            default:
                throw std::invalid_argument("error while parsing file by loading object with id: " + line);
            }
        }

        input_file.close();
    }

    return objects;
}

void LevelEditor::interpret_wooden_floor(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
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

void LevelEditor::interpret_decoy(std::string& line, std::ifstream& input_file, std::vector<Object*>& objects)
{
    throw std::invalid_argument("tried to interpret a decoy"); // we shouldnt be running into any of these. the decoy represent where the player should be - thats it
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
    // move back the objects
    for (Object* o : objects)
        o->move(total_panned * -1);

    std::ofstream output_file(file_path);

    if (output_file.is_open())
    {
        // write the player data
        output_file << std::to_string(static_cast<float>(player_position.x) / H_UNIT) << '\n' << std::to_string(static_cast<float>(player_position.y) / V_UNIT) << '\n'
                    << std::to_string(static_cast<float>(player_left_bound) / H_UNIT) << '\n' << std::to_string(static_cast<float>(player_right_bound) / H_UNIT) << '\n'
                    << std::to_string(static_cast<float>(player_upper_bound) / V_UNIT) << '\n' << std::to_string(static_cast<float>(player_lower_bound) / H_UNIT);

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
        o->move(total_panned);
}


LevelEditor::~LevelEditor()
{
    for (Object* o : objects)
        delete o;
}


int LevelEditor::get_valid_int(const std::string& error_message, std::string& line_buffer)
{
    int num{0};
    while (true)
    {
        try
        {
            num = std::stoi(line_buffer);
            return num;
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << error_message;
            std::cin >> line_buffer;
        }
    }
}


int LevelEditor::get_valid_int(const std::string& error_message, std::string& line_buffer, int lower_bound, int upper_bound)
{
    int num{0};
    while (true)
    {
        try
        {
            num = std::stoi(line_buffer);
            if (num < lower_bound || num >= upper_bound)
                throw std::invalid_argument("");
            return num;
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << error_message;
            std::cin >> line_buffer;
        }
    }
}


void LevelEditor::update()
{
    if (Input::GetKeyDown(Key::ENTER) && Input::GetKeyFrame(Key::ENTER) == 0)
    {
        save();
        return;
    }

    if (Input::GetKeyDown(Key::SPACE) && Input::GetKeyFrame(Key::SPACE) == 0)
        object_menu_open = !object_menu_open;

    if (object_menu_open)
    {
        if (Input::GetKeyDown(Key::E) && Input::GetKeyFrame(Key::E) == 0)
            edit_object_properties_from_menu();
        
        if (Input::GetLeftMouseDown() && Input::GetLeftMouseFrame() == 0)
            create_object();

        if (Input::GetKeyDown(Key::DOWN) && Input::GetKeyFrame(Key::DOWN) == 0)
            object_selected_from_menu = static_cast<Object::TYPE>((static_cast<int>(object_selected_from_menu) + 1) % static_cast<int>(Object::TYPE::COUNT));

        if (Input::GetKeyDown(Key::UP) && Input::GetKeyFrame(Key::UP) == 0)
        {
            int decremented_object = static_cast<int>(object_selected_from_menu) - 1;
            if (decremented_object == -1)
                object_selected_from_menu = static_cast<Object::TYPE>(static_cast<int>(Object::TYPE::COUNT) - 1);
            
            else
                object_selected_from_menu = static_cast<Object::TYPE>(decremented_object);
        }
    }
    else
    {
        update_selection();
        if (selected)
            move_selected_object();

    }

    

    update_pan();
}

void LevelEditor::update_selection()
{
    if (Input::GetKeyDown(Key::ESCAPE) && Input::GetKeyFrame(Key::ESCAPE) == 0)
    {
        selected = false;
        object_selected = objects.end();
        return;
    }
    if (Input::GetKeyDown(Key::DEL) && Input::GetKeyFrame(Key::DEL) == 0)
    {
        selected = false;
        objects.erase(object_selected);
        object_selected = objects.end();
        return;
    }
    
    if (Input::GetLeftMouseDown() && Input::GetLeftMouseFrame() == 0)
    {
        
        auto i = object_selected;
        if (i == objects.end() || ++i == objects.end())
            i = objects.begin();
        while (i != object_selected)
        {
            if (Input::GetLeftMousePosition().x >= (*i)->get_x() && Input::GetLeftMousePosition().x <= (*i)->get_x() + (*i)->get_width() &&
                Input::GetLeftMousePosition().y >= (*i)->get_y() && Input::GetLeftMousePosition().y <= (*i)->get_y() + (*i)->get_height())
            { // use aabb to see if we click something
                object_selected = i;
                selected = true;
                return;
            }

            if (++i == objects.end() && i != object_selected)
                i = objects.begin();
        }
        // we didnt find anything before we made it back to the first object
        selected = false;
        object_selected = objects.end();
    }
}

void LevelEditor::update_pan()
{
    Vec2I amount_to_move(0, 0);

    // amount_to_move.x -= PAN_SPEED * (Input::GetKeyDown(Key::D) - Input::GetKeyDown(Key::A)); // should do same thing but idk and its less readable

    if (Input::GetKeyDown(Key::A))
        amount_to_move.x += PAN_SPEED;

    if (Input::GetKeyDown(Key::D))
        amount_to_move.x -= PAN_SPEED;

    if (Input::GetKeyDown(Key::W))
        amount_to_move.y += PAN_SPEED;
    
    if (Input::GetKeyDown(Key::S))
        amount_to_move.y -= PAN_SPEED;

    if (!amount_to_move)
        return;
    
    if (Input::GetKeyDown(Key::SHIFT))
        amount_to_move *= SPEED_MULTIPLIER;
        

    for (Object* o : objects)
        o->move(amount_to_move);

    total_panned += amount_to_move;
}

void LevelEditor::move_selected_object()
{
    Vec2I amount_to_move(0, 0);

    if (Input::GetKeyDown(Key::LEFT))
        amount_to_move.x -= OBJECT_MOVE_SPEED;

    if (Input::GetKeyDown(Key::RIGHT))
        amount_to_move.x += OBJECT_MOVE_SPEED;

    if (Input::GetKeyDown(Key::UP))
        amount_to_move.y -= OBJECT_MOVE_SPEED;
    
    if (Input::GetKeyDown(Key::DOWN))
        amount_to_move.y += OBJECT_MOVE_SPEED;


    if (Input::GetKeyDown(Key::SHIFT))
        amount_to_move *= SPEED_MULTIPLIER;

    (*object_selected)->move(amount_to_move);

    if (object_selected == objects.begin())
        player_position += amount_to_move; // move the player if the decoy was selected
}


void LevelEditor::create_object()
{
    switch (object_selected_from_menu)
    {
    case Object::TYPE::WOODEN_FLOOR:
        objects.push_back(new WoodenFloor(Input::GetLeftMousePosition().x, Input::GetLeftMousePosition().y, wooden_floor_width, wooden_floor_height));
        break;
    case Object::TYPE::SPIKE:
        objects.push_back(new Spike(spike_rotation, Input::GetLeftMousePosition().x, Input::GetLeftMousePosition().y));
        break;
    case Object::TYPE::INVISIBLE_BOUNDRY:
        objects.push_back(new InvisibleBoundry(invisible_boundry_side, Input::GetLeftMousePosition().x, Input::GetLeftMousePosition().y, invisible_boundry_length));
        break;
    default:
        throw std::invalid_argument("trying to create object with an invalid ID");
    }
    object_selected = std::prev(objects.end());
    selected = true;
}


void LevelEditor::edit_object_properties_from_menu()
{
    std::string line;
    switch (object_selected_from_menu)
    {
    case Object::TYPE::WOODEN_FLOOR:
        std::cout << "Width: ";
        std::cin >> line;
        
        if (!line.empty())
            wooden_floor_width = get_valid_int("Invalid width -- try again: ", line);
        

        std::cout << "Height: ";
        std::cin >> line;

        if (!line.empty())
            wooden_floor_height = get_valid_int("Invalid height -- try again: ", line);

        break;
    case Object::TYPE::SPIKE:
        std::cout << "Enter rotation: left [0], right [1], up [2], down [3]: ";
        std::cin >> line;

        if (!line.empty())
            spike_rotation = static_cast<Spike::Facing>(get_valid_int("Rotation must be a number: left [0], right [1], up [2], down [3]: ", line, 0, 4));

        break;
    case Object::TYPE::INVISIBLE_BOUNDRY:
        std::cout << "Enter side: left [0], right [1], top [2], bottom [3]: ";
        std::cin >> line;

        if (!line.empty())
            invisible_boundry_side = static_cast<Side>(get_valid_int("Side must be a number: left [0], right [1], top [2], bottom [3]: ", line, 0, 4));

        std::cout << "Enter length for side: ";
        std::cin >> line;
        if (!line.empty())
            invisible_boundry_length = get_valid_int("Length must be a valid number: ", line);

        break;
    default:
        throw std::invalid_argument("invalid object id to edit: " + std::to_string(static_cast<int>(object_selected_from_menu)));
    }
}


void LevelEditor::draw(Graphics& g) const
{ // TODO
    for (Object* o : objects)
        o->draw(g);

    if (object_menu_open)
    {
        g.SetColor(D2D1::ColorF::AntiqueWhite);
        g.FillRect(D2D1::RectF(36_hu, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

        g.DrawBitmap(object_menu_sprites[static_cast<int>(object_selected_from_menu)], object_menu_sprite_bounds[static_cast<int>(object_selected_from_menu)], { 39.0_hu, 13.0_vu, 45.0_hu, 19.0_vu});
        g.DrawBitmap(object_menu_sprites[(static_cast<int>(object_selected_from_menu) + 1) % static_cast<int>(Object::TYPE::COUNT)], object_menu_sprite_bounds[(static_cast<int>(object_selected_from_menu) + 1) % static_cast<int>(Object::TYPE::COUNT)], { 40.0_hu, 23.0_vu, 45.0_hu, 28.0_vu});
        int next_object = static_cast<int>(object_selected_from_menu) - 1;
        if (next_object < 0)
            next_object = static_cast<int>(Object::TYPE::COUNT) - 1;

        g.DrawBitmap(object_menu_sprites[next_object], object_menu_sprite_bounds[next_object], { 40.0_hu, 5_vu, 45.0_hu, 9.0_vu });

        g.SetColor(D2D1::ColorF::Black);
        g.DrawRect(D2D1::RectF(39.0_hu, 13.0_vu, 45.0_hu, 19.0_vu), 3.0f);
    }
    else if (selected)
    {
        g.SetColor(D2D1::ColorF::LimeGreen);
        const D2D1_POINT_2F p1 = D2D1::Point2F((*object_selected)->get_x(), (*object_selected)->get_y());
        const D2D1_POINT_2F p2 = D2D1::Point2F(p1.x + (*object_selected)->get_width(), p1.y);
        const D2D1_POINT_2F p3 = D2D1::Point2F(p2.x, p2.y + (*object_selected)->get_height());
        const D2D1_POINT_2F p4 = D2D1::Point2F(p1.x, p3.y);

        constexpr static float OUTLINE_THICKNESS = 3.0f;
        g.DrawLine(p1, p2, OUTLINE_THICKNESS);
        g.DrawLine(p2, p3, OUTLINE_THICKNESS);
        g.DrawLine(p4, p3, OUTLINE_THICKNESS);
        g.DrawLine(p1, p4, OUTLINE_THICKNESS);
    }
}

ID2D1Bitmap* LevelEditor::object_menu_sprites[static_cast<int>(Object::TYPE::COUNT)];
D2D1_RECT_F LevelEditor::object_menu_sprite_bounds[static_cast<int>(Object::TYPE::COUNT)];

HRESULT LevelEditor::init_resources(Graphics& g)
{
    HRESULT hr = WoodenFloor::init(g);

    if (SUCCEEDED(hr))
        hr = Spike::init(g);

    if (SUCCEEDED(hr))
        hr = Decoy::init(g);

    for (int i = 0; i < static_cast<int>(Object::TYPE::COUNT); ++i)
    {
        if (SUCCEEDED(hr))
        {
            switch (static_cast<Object::TYPE>(i))
            {
            case Object::TYPE::WOODEN_FLOOR:
                hr = g.LoadBitmapFromFile(L".\\images\\wooden_floor.png", &object_menu_sprites[i]);
                object_menu_sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            case Object::TYPE::SPIKE:
                hr = g.LoadBitmapFromFile(L".\\images\\spikes.png", &object_menu_sprites[i]);
                object_menu_sprite_bounds[i] = { 64.0f, 0.0f, 96.0f, 32.0f };
                break;
            case Object::TYPE::INVISIBLE_BOUNDRY:
                hr = g.LoadBitmapFromFile(L".\\images\\red_line.png", &object_menu_sprites[i]);
                object_menu_sprite_bounds[i] = { 0.0f, 0.0f, 32.0f, 32.0f };
                break;
            default:
                throw std::invalid_argument("trying to initialize a sprite with an invalid type");
            }
        } else {
            std::cout << i << '\n';
        }
    }

    return hr;
}