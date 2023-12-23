#pragma once

#include <string>


namespace IoAssistance
{

int get_valid_int(const std::string& error_message, std::string& line_buffer);
int get_valid_int(const std::string& error_message, std::string& line_buffer, int lower_bound, int upper_bound);
//                                                                              included          excluded
}