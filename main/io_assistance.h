#pragma once

#include <string>


namespace IoAssistance
{
int get_y_or_n();

int get_valid_int(const std::string& error_message, std::string& line_buffer);
int get_valid_int(const std::string& error_message, std::string& line_buffer, int lower_bound, int upper_bound);
//                                                                              included          excluded
float get_valid_float(const std::string& error_message, std::string& line_buffer);
float get_valid_float(const std::string& error_message, std::string& line_buffer, float lower_bound, bool lower_included, float upper_bound, bool upper_included);
}