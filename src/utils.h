#pragma once

#include <string>
#include <vector>

std::string read_file(std::string path);

std::vector<std::string> read_lines(std::string input);

std::vector<std::string> split(std::string input, char delimiter);

float string_to_float(std::string input);
