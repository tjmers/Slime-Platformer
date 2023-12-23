#pragma once

#include "action.h"

template <typename T, int MAX_CAPACITY>
class StackMaxCapacity
{
public:
    StackMaxCapacity();
    void push(const T& element);
    void push(T&& element);
    void pop();
    T& peek();

    inline int size() const { return length; }
    inline bool empty() const { return length == 0; }

private:
    T data[MAX_CAPACITY];
    int length;
    int current;
};

// template class StackMaxCapacity<char, 10>; // used for testing in tests/stack_test.cpp
template class StackMaxCapacity<Action, 100>;