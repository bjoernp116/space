#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string read_file(std::string path) {
	std::string content;
	std::ifstream stream(path);

	if (!stream.good()) {
		std::cerr << "Failed to open file " << path << std::endl;
		return "";
	} else {
		std::ostringstream ss;
		ss << stream.rdbuf();
		stream.close();
		return ss.str();
	}
}

std::vector<std::string> read_lines(std::string input) {
	std::vector<std::string> lines;
	std::istringstream stream(input);
	std::string line;
	while (std::getline(stream, line)) {
		lines.push_back(line);
	}
	return lines;
}

std::vector<std::string> split(std::string input, char delimiter) {
	std::vector<std::string> result;
	std::string current;

	for (char ch : input) {
		if (ch == delimiter) {
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		} else {
			current += ch;
		}
	}

	if (!current.empty()) {
		result.push_back(current);
	}

	return result;
}
