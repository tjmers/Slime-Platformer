#include "level_editor.h"

#include <bit>
#include <fstream>
#include <iostream>

#include "../main/input.h"



LevelEditor::LevelEditor()
    : file_path(get_file_path()), player_position(0.0f, 0.0f), objects({new Decoy(0, 0)}),
      player_bounds({ 1.0_hu, 1.0_vu, SCREEN_WIDTH - 1.0_hu, SCREEN_HEIGHT - 1.0_vu }), draw_player_bounds(true),
      total_panned(0, 0), selected(false), object_selected(objects.end()),
      object_menu_open(false), object_selected_from_menu(static_cast<Object::TYPE>(0)),
      undos(), deleted_objects(),
      wooden_floor_width(100), wooden_floor_height(100),
      spike_rotation(Spike::Facing::UP),
      invisible_boundry_side(Side::LEFT), invisible_boundry_length(0)
{}

LevelEditor::LevelEditor(const std::string& file_path)
    : file_path(std::move(file_path)), player_position(LevelLoader::get_player_position(this->file_path)), objects(LevelLoader::get_objects(this->file_path, player_position)),
      player_bounds(LevelLoader::get_player_bounds(this->file_path)), draw_player_bounds(true),
      total_panned(0, 0), selected(false), object_selected(objects.end()),
      object_menu_open(false), object_selected_from_menu(static_cast<Object::TYPE>(0)),
      undos(), deleted_objects(),
      wooden_floor_width(100), wooden_floor_height(100),
      spike_rotation(Spike::Facing::UP),
      invisible_boundry_side(Side::LEFT)
{}

LevelEditor::LevelEditor(Vec2F player_position, float player_left_bound, float player_right_bound, float player_upper_bound, float player_lower_bound, std::vector<Object*>& objects)
    : player_position(player_position), player_bounds({ player_left_bound, player_upper_bound, player_right_bound, player_right_bound }), draw_player_bounds(true),
      objects(objects), file_path(get_file_path()), total_panned(0, 0),
      selected(false), object_selected(objects.end()), object_menu_open(false), object_selected_from_menu(static_cast<Object::TYPE>(0)),
      undos(), deleted_objects(),
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



LevelEditor::~LevelEditor()
{
    for (Object* o : objects)
        delete o;
    
    while (!undos.empty())
    {
        Action a = undos.peek();
        a.cleanup(a.param);
        undos.pop();
    }
}



void LevelEditor::update()
{
    if (Input::GetKeyDown(Key::ENTER) && Input::GetKeyFrame(Key::ENTER) == 0)
    {
        LevelLoader::save_objects(file_path, objects, total_panned, player_position, player_bounds);
        return;
    }

    if (Input::GetKeyDown(Key::CTRL) && Input::GetKeyDown(Key::Z) && (Input::GetKeyDown(Key::SHIFT) || Input::GetKeyFrame(Key::Z) == 0) && !undos.empty())
    {
        undo();
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
        else if (Input::GetKeyDown(Key::E) && Input::GetKeyFrame(Key::E) == 0)
            edit_level_properties();

    }

    

    update_pan();
}

void LevelEditor::undo()
{
    undos.peek().undo(undos.peek().param);
    undos.pop();
}

void LevelEditor::update_selection()
{
    if (Input::GetKeyDown(Key::ESCAPE) && Input::GetKeyFrame(Key::ESCAPE) == 0)
    {
        if (selected)
        {
            add_selected_object_change_to_undos();
            selected = false;
            object_selected = objects.end();
        }
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
                if (selected)
                    add_selected_object_change_to_undos();
                else
                {
                    undos.push(Action(
                        [this] (Action::Param&)
                        {
                            selected = false;
                            object_selected = objects.end();
                        }
                    ));
                    selected = true;
                }
                object_selected = i;
                return;
            }

            if (++i == objects.end() && i != object_selected)
                i = objects.begin();
        }
        // we didnt find anything before we made it back to the first object
        if (selected)
        {
            add_selected_object_change_to_undos();
            selected = false;
            object_selected = objects.end();
        }
    }
}

void LevelEditor::add_selected_object_change_to_undos()
{
    undos.push(Action(
        [this] (Action::Param& object_address)
        {
            Object* target_object = reinterpret_cast<Object*>(object_address.eightByte);
            object_selected = std::find(objects.begin(), objects.end(), target_object);

            if (object_selected == objects.end())
                throw std::invalid_argument("cannot find the object to make selected (Input::GetKeyDown(Key::ESCAPE))");

            selected = true;
        }, Action::Param(std::bit_cast<long long>(*object_selected))
    ));
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
    if (Input::GetKeyDown(Key::DEL) && Input::GetKeyFrame(Key::DEL) == 0)
    {
        selected = false;
        undos.push(Action(
            [this] (Action::Param& iterator_to_insert_to)
            {
                auto position = std::bit_cast<std::vector<Object*>::iterator>(iterator_to_insert_to);
                if (deleted_objects.empty())
                    throw std::out_of_range("trying to bring back an object but the deleted objects deque is empty");

                objects.insert(position, deleted_objects.back());
                object_selected = position;
                selected = true;
            },
            [this] (Action::Param&)
            {
                if (objects.empty())
                    throw std::out_of_range("trying to cleanup the deleted objects but its empty");
                delete deleted_objects.front();
                deleted_objects.pop_front();
            }, Action::Param(std::bit_cast<long long>(object_selected))
        ));
        deleted_objects.push_back(*object_selected);
        objects.erase(object_selected);
        object_selected = objects.end();
        return;
    }


    if (Input::GetKeyDown(Key::E) && Input::GetKeyFrame(Key::E) == 0)
    {
        // adding an undo to this would be complicated. so we wont.
        std::cout << "Editing Object (-) to keep value same\n";
        (*object_selected)->edit();
        std::cout << "Edit complete\n";
        return;
    }


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

    check_front_ordering();

    check_back_ordering();

    if (!amount_to_move)
        return;

    (*object_selected)->move(amount_to_move);
    

    undos.push(Action([this, amount_to_move] (Action::Param&) {
        (*object_selected)->move(amount_to_move * -1);
    }));
    
  
    if (object_selected == objects.begin())
    {
        player_position += amount_to_move; // move the player if the decoy was selected
        return; // make sure that we dont move the order of the decoy
    }


}

void LevelEditor::check_front_ordering()
{
    if (Input::GetKeyDown(Key::LBRACKET) && Input::GetKeyFrame(Key::LBRACKET) == 0 && object_selected != std::next(objects.begin()))
    { // move back in ordering, towards the front of the object vector
        if (Input::GetKeyDown(Key::SHIFT))
        {   // move to the index 1, shifting everything else over
            undos.push(Action(
            [this] (Action::Param& object_address)
            {
                Object* object = reinterpret_cast<Object*>(object_address.eightByte);
                auto i = std::find(std::next(objects.begin()), objects.end(), object);
                if (i == objects.end())
                    throw std::out_of_range("did not find object to move back in the ordering");
                auto new_location = std::prev(objects.end());
                std::iter_swap(i, new_location);
                object_selected = new_location;

            }, Action::Param(std::bit_cast<long long>(*object_selected))
            ));
            std::iter_swap(object_selected, std::next(objects.begin()));
            object_selected = std::next(objects.begin());
        }
        else
        {
            std::iter_swap(object_selected, std::prev(object_selected));
            --object_selected;

            undos.push(Action([this] (Action::Param&) {
                std::iter_swap(object_selected, std::next(object_selected));
                ++object_selected;
            }));
        }
    }
}

void LevelEditor::check_back_ordering()
{
    if (Input::GetKeyDown(Key::RBRACKET) && Input::GetKeyFrame(Key::RBRACKET) == 0 && object_selected != std::prev(objects.end()))
    { // move forwards in ordering, towards the back of the object vector

        if (Input::GetKeyDown(Key::SHIFT))
        {   // move to the last index, shifting everything over
            undos.push(Action(
                [this] (Action::Param& object_address)
                {
                // move the element in the end to the element address stored in object_address
                Object* object = reinterpret_cast<Object*>(object_address.eightByte);
                auto i = std::find(std::next(objects.begin()), objects.end(), object);
                if (i == objects.end())
                    throw std::out_of_range("did not find object to move fowards in the ordering");
                auto new_location = std::next(objects.begin());
                std::iter_swap(i, new_location);
                object_selected = new_location;
                
            }, Action::Param(std::bit_cast<long long>(*object_selected))));

            std::iter_swap(object_selected, std::prev(objects.end()));
            object_selected = std::prev(objects.end());
        }
        else
        {
            std::iter_swap(object_selected, std::next(object_selected));
            ++object_selected;

            undos.push(Action(
            [this] (Action::Param&)
            {
                std::iter_swap(object_selected, std::prev(object_selected));
                --object_selected;
            }));
        }
        
    }
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
    if (selected)
        undos.push(Action(
            [this] (Action::Param& previously_selected_object)
            {
                delete objects[objects.size() - 1];
                Object* object_to_be_selected = reinterpret_cast<Object*>(previously_selected_object.eightByte);
                auto i = std::find(objects.begin(), objects.end(), object_to_be_selected);
                if (i == objects.end())
                    throw std::out_of_range("cannot find object to make selected");

                object_selected = i;
                objects.pop_back();
            }, Action::Param(std::bit_cast<long long>(*object_selected))
        ));
    else
        undos.push(Action(
            [this] (Action::Param&)
            {
                delete objects[objects.size() - 1];
                objects.pop_back();
                selected = false;
            }
        ));
    object_selected = std::prev(objects.end());
    selected = true;
}


void LevelEditor::edit_object_properties_from_menu()
{
    std::string line;
    std::cout << "Enter a dash '-' to keep the same value\n";
    switch (object_selected_from_menu)
    {
    case Object::TYPE::WOODEN_FLOOR:
        std::cout << "Width (" << wooden_floor_width << "): ";
        std::cin >> line;
        
        if (line != "-")
            wooden_floor_width = IoAssistance::get_valid_int("Invalid width -- try again: ", line);
        

        std::cout << "Height (" << wooden_floor_height << "): ";
        std::cin >> line;

        if (line != "-")
            wooden_floor_height = IoAssistance::get_valid_int("Invalid height -- try again: ", line);

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
    default:
        throw std::invalid_argument("invalid object id to edit: " + std::to_string(static_cast<int>(object_selected_from_menu)));
    }
    std::cout << "Object edited\n";
}

void LevelEditor::edit_level_properties()
{
    std::string line;
    std::cout << "Enter a dash '-' to keep the same value\n";

    std::cout << "Enter new value for left bound in number of h_units (was " + std::to_string(player_bounds.left / 1_hu) + ", max=" + std::to_string(NUM_H_UNIT) + "): ";
    std::cin >> line;
    if (line != "-")
        player_bounds.left = IoAssistance::get_valid_float("x value must be a floating point number: ", line) * H_UNIT;

    
    std::cout << "Enter new value for right bound in number of h_units (was " + std::to_string(player_bounds.right / 1_hu) + ", max=" + std::to_string(NUM_H_UNIT) + "): ";
    std::cin >> line;
    if (line != "-")
        player_bounds.right = IoAssistance::get_valid_float("x value must be a floating point number: ", line) * H_UNIT;

    
    std::cout << "Enter new value for upper bound in number of h_units (was " + std::to_string(player_bounds.top / 1_vu) + ", max=" + std::to_string(NUM_V_UNIT) + "): ";
    std::cin >> line;
    if (line != "-")
        player_bounds.top = IoAssistance::get_valid_float("y value must be a floating point number: ", line) * V_UNIT;

    
    std::cout << "Enter new value for lower bound in number of h_units (was " + std::to_string(player_bounds.bottom / 1_vu) + ", max=" + std::to_string(NUM_V_UNIT) + "): ";
    std::cin >> line;
    if (line != "-")
        player_bounds.bottom = IoAssistance::get_valid_float("y value must be a floating point number: ", line) * V_UNIT;

    std::cout << "Should the player bounds be drawn [y/n]: ";
    int should_draw_lines_input = IoAssistance::get_y_or_n();

    if (should_draw_lines_input == 'y')
        draw_player_bounds = true;
    else
        draw_player_bounds = false;
    
}


void LevelEditor::draw(Graphics& g) const
{
    for (auto i = std::next(objects.begin()); i != objects.end(); ++i)
        (*i)->draw(g);
    objects[0]->draw(g);

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
        
        constexpr static float OUTLINE_THICKNESS = 3.0f;
        
        g.DrawRect(D2D1::RectF((*object_selected)->get_x(), (*object_selected)->get_y(),
                               (*object_selected)->get_x() + (*object_selected)->get_width(), (*object_selected)->get_y() + (*object_selected)->get_height()),
                                OUTLINE_THICKNESS);
    }

    if (draw_player_bounds)
    {
        g.SetColor(D2D1::ColorF(0.3f, 0.3f, 0.3f, 0.3f));
        g.DrawRect(player_bounds, 2.0f);
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