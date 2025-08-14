#pragma once
#include <glad/glad.h>

#include <string>
#include <vector>

std::string read_file(std::string path);

std::vector<std::string> read_lines(std::string input);

std::vector<std::string> split(std::string input, char delimiter);

float string_to_float(std::string input);

void GLAPIENTRY debug_callback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam);

class Log {
	static void debug(std::string str);
	static void error(std::string str);
	static void warning(std::string str);
};
