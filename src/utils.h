#pragma once

#define GL_ERR()                                                               \
	do {                                                                       \
		GLenum err;                                                            \
		while ((err = glGetError()) != GL_NO_ERROR) {                          \
			std::cerr << "OpenGL error: " << err << " at " << __FILE__ << ":"  \
			          << __LINE__ << std::endl;                                \
		}                                                                      \
	} while (0)

#include <string>

std::string read_file(std::string path);
