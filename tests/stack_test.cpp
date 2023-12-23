// compilation command: g++ tests/stack_test.cpp creator/stack_map_capacity.cpp -o tests/stack_test.exe

#include <iostream>
#include "../creator/stack_max_capacity.h"


int main(void)
{
    StackMaxCapacity<char, 10> stack;

    for (int i = 0; i < 11; ++i)
        stack.push(i + 'a');

    while (!stack.empty())
    {
        std::cout << stack.peek();
        stack.pop();
    }

    return 0;
}