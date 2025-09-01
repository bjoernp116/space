#include "file.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

File::File(const char *path) : file(path), path(path) {
	open = file.is_open();
}

File::File(std::string path) : file(path), path(path) {
    spdlog::debug("File {}: opened!", path);
	open = file.is_open();
}

// void File::write_line(const char* line);

const std::string File::read() const {
	if (open) {
        std::stringstream str;
		str << file.rdbuf();
        return str.str();
	}
	spdlog::error("File {}: read before open!", path);
    std::exit(-1);
}

const std::vector<std::string> File::read_lines() const {
	std::vector<std::string> lines;
    std::string content = read();

    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}
