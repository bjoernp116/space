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
