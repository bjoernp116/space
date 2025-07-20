#include "shader.h"
#include "utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

const std::string SHADER_PATH = "./shaders/";

Shader::Shader(std::string path, GLenum shader_type) {
	src = read_file(SHADER_PATH + path);
	type = shader_type;
	id = glCreateShader(shader_type);
	GL_ERR();
}

Shader::~Shader() {
	glDeleteShader(id);
	GL_ERR();
}

void Shader::compile_shader() const {
	std::cout << src;
	const char *src_point = src.c_str();
	glShaderSource(id, 1, &src_point, nullptr);
	GL_ERR();
	glCompileShader(id);
	GL_ERR();
}

bool Shader::status() const {
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
		          << infoLog << std::endl;
		std::cout << "(" << src << ")" << std::endl;

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
	GL_ERR();
}

void ShaderProgram::set_vector3f(
    const char *name, float x, float y, float z) const {
	int location = location_table.at(name);
	if (location == -1) {
		std::cerr << "Uniform " << name << " does not exist!" << std::endl;
	}
	glUniform3f(location, x, y, z);
	GL_ERR();
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
	GL_ERR();
	glAttachShader(id, fragment.id);
	GL_ERR();
	glLinkProgram(id);
	GL_ERR();

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cerr << "SHADER PROGRAM LINK FAILED:\n" << infoLog << std::endl;
	}

	glDetachShader(id, vertex.id);
	GL_ERR();
	glDetachShader(id, fragment.id);
	GL_ERR();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
	GL_ERR();
}

void ShaderProgram::use() const {
	glUseProgram(id);
	GL_ERR();
}
