#pragma once

#include "../main/object.h"


class InvisibleBoundry : public Object
{
public:
    InvisibleBoundry(Side side, int x, int y, int length);
    void write_to_file(std::ofstream& output_file) const override;
};