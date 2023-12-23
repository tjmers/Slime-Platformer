#include "stack_max_capacity.h"

// #include <iostream>j
#include <stdexcept>
#include <utility>


template <typename T, int MAX_CAPACITY>
StackMaxCapacity<T, MAX_CAPACITY>::StackMaxCapacity()
    : length(0), current(0), data() {}


template <typename T, int MAX_CAPACITY>
void StackMaxCapacity<T, MAX_CAPACITY>::push(const T& element)
{
    ++current;
    if (current == MAX_CAPACITY)
        current = 0;

    data[current] = element;

    if (length != MAX_CAPACITY)
        ++length;
}

template <typename T, int MAX_CAPACITY>
void StackMaxCapacity<T, MAX_CAPACITY>::push(T&& element)
{
    // std::cout << 1 << '\n';
    ++current;
    if (current == MAX_CAPACITY)
        current = 0;
    // std::cout << 2 << '\n';
    data[current] = std::move(element);
    // std::cout << 3 << '\n';
    if (length != MAX_CAPACITY)
        ++length;
    // std::cout << 4 << '\n';
}

template <typename T, int MAX_CAPACITY>
void StackMaxCapacity<T, MAX_CAPACITY>::pop()
{ // just dont call this if the stack is empty
// #if __OPTIMZATION_LEVEL__ == 0
//     if (empty())
//         throw std::invalid_argument("cannot pop an empty stack");
// #endif

    if (current == 0)
        current = MAX_CAPACITY - 1;
    else
        --current;
    --length;
}

template <typename T, int MAX_CAPACITY>
T& StackMaxCapacity<T, MAX_CAPACITY>::peek()
{
// #if __OPTIMZATION_LEVEL__ == 0
//     if (empty())
//         throw std::invalid_argument("cannot peek an empty stack");
// #endif
    return data[current];
}