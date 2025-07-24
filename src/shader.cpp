#include <spdlog/spdlog.h>
#include "shader.h"
#include "utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

const std::string SHADER_PATH = "./shaders/";

Shader::Shader(std::string path, GLenum shader_type) {
	src = read_file(SHADER_PATH + path);
	type = shader_type;
	id = glCreateShader(shader_type);
}

Shader::~Shader() {
	glDeleteShader(id);
}

void Shader::compile_shader() const {
	const char *src_point = src.c_str();
	glShaderSource(id, 1, &src_point, nullptr);

	glCompileShader(id);
}

bool Shader::status() const {
	int success;
	char info_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, info_log);
		spdlog::error("Shader compilation error: {0}",
		    reinterpret_cast<const char *>(info_log));
		spdlog::debug("({0})", src);

		return false;
	}
	return true;
}

void ShaderProgram::declare(std::vector<const char *> identifiers) {
	for (auto &identifier : identifiers) {
		int location = glGetUniformLocation(id, identifier);
		location_table.insert(std::make_pair(identifier, location));
	}
}

void ShaderProgram::set_matrix4(const char *name,
    const glm::mat4 &matrix) const {
	int location = location_table.at(name);
	if (location == -1) {
		std::cerr << "Uniform " << name << " does not exist!" << std::endl;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::set_vector3f(
    const char *name, float x, float y, float z) const {
	int location = location_table.at(name);
	if (location == -1) {
		std::cerr << "Uniform " << name << " does not exist!" << std::endl;
	}
	glUniform3f(location, x, y, z);
}

ShaderProgram::ShaderProgram(std::string vertex_path,
    std::string fragment_path) {
	Shader vertex(vertex_path, GL_VERTEX_SHADER);
	vertex.compile_shader();
	vertex.status();
	Shader fragment(fragment_path, GL_FRAGMENT_SHADER);
	fragment.compile_shader();
	fragment.status();

	id = glCreateProgram();
	glAttachShader(id, vertex.id);

	glAttachShader(id, fragment.id);

	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cerr << "SHADER PROGRAM LINK FAILED:\n" << infoLog << std::endl;
	}

	glDetachShader(id, vertex.id);

	glDetachShader(id, fragment.id);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
}

void ShaderProgram::use() const {
	glUseProgram(id);
}
