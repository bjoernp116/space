#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <spdlog/spdlog.h>

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

void GLAPIENTRY debug_callback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;
	spdlog::error("Opengl Error {0}: {1}", id, message);
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		spdlog::debug("Source: API");
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		spdlog::debug("Source: Window System");
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		spdlog::debug("Source: Shader Compiler");
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		spdlog::debug("Source: Third Party");
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		spdlog::debug("Source: Application");
		break;
	case GL_DEBUG_SOURCE_OTHER:
		spdlog::debug("Source: Other");
		break;
	}
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		spdlog::debug("Type: Error");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		spdlog::debug("Type: Deprecated Behaviour");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		spdlog::debug("Type: Undefined Behaviour");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		spdlog::debug("Type: Portability");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		spdlog::debug("Type: Performance");
		break;
	case GL_DEBUG_TYPE_MARKER:
		spdlog::debug("Type: Marker");
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		spdlog::debug("Type: Push Group");
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		spdlog::debug("Type: Pop Group");
		break;
	case GL_DEBUG_TYPE_OTHER:
		spdlog::debug("Type: Other");
		break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		spdlog::debug("Severity: high");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		spdlog::debug("Severity: medium");
		break;
	case GL_DEBUG_SEVERITY_LOW:
		spdlog::debug("Severity: low");
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		spdlog::debug("Severity: notification");
		break;
	}
}
