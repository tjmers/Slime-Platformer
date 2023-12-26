#pragma once

#include "../main/object.h"


class InvisibleBoundry : public Object
{
public:
    InvisibleBoundry(Side side, int x, int y, int length);
#ifdef LEVEL_EDITOR
    InvisibleBoundry(const InvisibleBoundry& other);
    Object* clone() const override;
    void write_to_file(std::ofstream& output_file) const override;
    int get_x() const override;
    int get_y() const override;
    int get_width() const override; // we give it a width and height of 5 so we can click it without needing pixel perfect accuracy
    int get_height() const override;
    void edit(StackMaxCapacity<Action, 1000>& undos) override;
#endif
};