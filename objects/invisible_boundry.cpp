#include "invisible_boundry.h"

#ifdef LEVEL_EDITOR
#include "../main/io_assistance.h"
#endif


InvisibleBoundry::InvisibleBoundry(Side side, int x, int y, int length)
    : Object({Collidable(side, x, y, length)}, {}) {}


#ifdef LEVEL_EDITOR

InvisibleBoundry::InvisibleBoundry(const InvisibleBoundry& other)
    : Object({Collidable(other.collidables[0].get_side(), other.collidables[0].get_x(), other.collidables[0].get_y(), other.collidables[0].get_length())}, {}) {}

Object* InvisibleBoundry::clone() const { return new InvisibleBoundry(*this); }

void InvisibleBoundry::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(static_cast<int>(Object::TYPE::INVISIBLE_BOUNDRY))
                << '\n' << std::to_string(static_cast<int>(collidables[0].get_side())) << '\n' << std::to_string(static_cast<float>(collidables[0].get_x()) / H_UNIT)
                << '\n' << std::to_string(static_cast<float>(collidables[0].get_y()) / V_UNIT) << '\n';

    if (collidables[0].get_side() == Side::TOP || collidables[0].get_side() == Side::BOTTOM)
    {
        output_file << std::to_string(static_cast<float>(collidables[0].get_length()) / H_UNIT);
    }
    else
    {
        output_file << std::to_string(static_cast<float>(collidables[0].get_length()) / V_UNIT);
    }
}

int InvisibleBoundry::get_x() const { return collidables[0].get_x(); }
int InvisibleBoundry::get_y() const { return collidables[0].get_y(); }
int InvisibleBoundry::get_width() const
{
    if (collidables[0].get_side() == Side::TOP || collidables[0].get_side() == Side::BOTTOM)
        return collidables[0].get_length();

    else
        return 5;
}

int InvisibleBoundry::get_height() const
{
    if (collidables[0].get_side() == Side::LEFT || collidables[0].get_side() == Side::RIGHT)
        return collidables[0].get_length();

    else
        return 5;
}


void InvisibleBoundry::edit(StackMaxCapacity<Action, 1000>& undos)
{
    undos.push(Action(
        [this] (Action::Param& data)
        {
            collidables[0].set_side(static_cast<Side>(data.four_byte1));
            collidables[0].set_length(data.four_byte2);
        }, Action::Param(static_cast<int>(collidables[0].get_side()), collidables[0].get_length())
    ));
    std::string line;
    std::cout << "Enter side: left [0], right [1], top [2], bottom [3]: ";
    std::cin >> line;

    if (line != "-")
        collidables[0].set_side(static_cast<Side>(IoAssistance::get_valid_int("Side must be a number: left [0], right [1], top [2], bottom [3]: ", line, 0, 4)));

    std::cout << "Enter length for side (" << collidables[0].get_length() << "): ";
    std::cin >> line;
    if (line != "-")
        collidables[0].set_length(IoAssistance::get_valid_int("Length must be a valid number: ", line));

}

#endif