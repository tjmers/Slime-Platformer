// compilation command: g++ tests/action_undo.cpp creator/stack_max_capacity.cpp creator/action.cpp -o tests/action_undo.exe

#include <iostream>

#include "../creator/action.h"
#include "../creator/stack_max_capacity.h"

void add_action(StackMaxCapacity<Action, 100>& undos)
{
    // undos.push(Action([](Action::Param&){std::cout << "Action undoing\n";}));
    Action a([](Action::Param&){std::cout << "Action undoing\n";});
    undos.push(std::move(a));
}

void undo_action(StackMaxCapacity<Action, 100>& undos)
{
    Action last = std::move(undos.peek());
    undos.pop();
    last.undo(last.param);
    last.cleanup(last.param);
}


int main(void)
{
    StackMaxCapacity<Action, 100> undos;
    add_action(undos);
    undo_action(undos);

    return 0;
}