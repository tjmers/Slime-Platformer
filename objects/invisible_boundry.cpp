#include "invisible_boundry.h"

InvisibleBoundry::InvisibleBoundry(Side side, int x, int y, int length)
    : Object({Collidable(side, x, y, length)}, {}) {}


void InvisibleBoundry::write_to_file(std::ofstream& output_file) const
{
    output_file << '\n' << std::to_string(static_cast<int>(collidables[0].get_side())) << '\n' << std::to_string(static_cast<float>(collidables[0].get_x()) / H_UNIT)
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