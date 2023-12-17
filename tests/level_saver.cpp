// compilation command: g++ tests\level_saver.cpp main\object.cpp main/graphics.cpp main/collidable.cpp main/vec2.cpp objects\invisible_boundry.cpp objects\spike.cpp objects\wooden_floor.cpp creator\level_editor.cpp -ld2d1 -lWindowsCodecs -lole32 -o tests/level_saver.exe

#include <iostream>

#include "../creator/level_editor.h"
#include "../objects/wooden_floor.h"

int main()
{
    std::vector<Object*> objects;
    objects.push_back(new WoodenFloor(0, 20_hu, 30_vu, 5_vu));
    LevelEditor editor({0, 0}, 0, 100_hu, 0, 100_vu, objects);
    editor.save();

    return 0;
}