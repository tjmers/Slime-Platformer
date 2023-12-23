// compilation command: g++ tests/misc_test.cpp -o tests/misc_test.exe

#include <iostream>
// #include <vector>

void fun(const int& i)
{
    int& i2 = const_cast<int&>(i);
    i2 = 0;
}

int main()
{
    // std::cout << sizeof(char*) << ' ' << sizeof(std::vector<char>::iterator) << '\n';
    int x = 2;
    std::cout << x << '\n';
    fun(x);
    std::cout << x << '\n';

    return 0;
}