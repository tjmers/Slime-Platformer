#include "io_assistance.h"

#include <iostream>
#include <stdexcept>

namespace IoAssistance
{

int get_y_or_n()
{
    int choice;
    do
    {
        choice = getchar();
        if (choice >= static_cast<int>('A') && choice <= static_cast<int>('Z'))
			choice += 32;
    }
    while (choice != static_cast<int>('y') && choice != static_cast<int>('n'));

    return choice;
}

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

float get_valid_float(const std::string& error_message, std::string& line_buffer)
{
    float num{0.f};
    while (true)
    {
        try
        {
            num = std::stof(line_buffer);
            return num;
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << error_message;
            std::cin >> line_buffer;
        }
    }


}

float get_valid_float(const std::string& error_message, std::string& line_buffer, float lower_bound, bool lower_included, float upper_bound, bool upper_included)
{
    if (lower_included && upper_included)
    {
        float num{0.f};
        while (true)
        {
            try
            {
                num = std::stof(line_buffer);
                if (num < lower_bound || num > upper_bound)
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
    else if (lower_included)
    {
        float num{0.f};
        while (true)
        {
            try
            {
                num = std::stof(line_buffer);
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
    else if (upper_included)
    {
        float num{0.f};
        while (true)
        {
            try
            {
                num = std::stof(line_buffer);
                if (num <= lower_bound || num > upper_bound)
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
    else
    {
        float num{0.f};
        while (true)
        {
            try
            {
                num = std::stof(line_buffer);
                if (num <= lower_bound || num >= upper_bound)
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

}