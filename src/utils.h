#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

std::string read_file(const char *path) {
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
