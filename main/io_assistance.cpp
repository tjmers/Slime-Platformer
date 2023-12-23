#include "io_assistance.h"

#include <iostream>
#include <stdexcept>

namespace IoAssistance
{


int get_valid_int(const std::string& error_message, std::string& line_buffer)
{
    int num{0};
    while (true)
    {
        try
        {
            num = std::stoi(line_buffer);
            return num;
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << error_message;
            std::cin >> line_buffer;
        }
    }
}

int get_valid_int(const std::string& error_message, std::string& line_buffer, int lower_bound, int upper_bound)
{
    int num{0};
    while (true)
    {
        try
        {
            num = std::stoi(line_buffer);
            if (num < lower_bound || num >= upper_bound)
                throw std::invalid_argument("");
            return num;
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << error_message;
            std::cin >> line_buffer;
        }
    }
}
}