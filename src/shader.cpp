#include "shader.h"
#include "utils.h"
#include <iostream>

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

void Shader::compile_shader() {
	const char *src_point = src.c_str();
	glShaderSource(id, 1, &src_point, nullptr);
	GL_ERR();
	glCompileShader(id);
	GL_ERR();
}

bool Shader::status() {
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

ShaderProgram::ShaderProgram(std::string vertex_path, std::string fragment_path)
    : vertex(vertex_path, GL_VERTEX_SHADER),
      fragment(fragment_path, GL_FRAGMENT_SHADER) {

	vertex.compile_shader();
	fragment.compile_shader();

	id = glCreateProgram();
	glAttachShader(id, vertex.id);
	GL_ERR();
	glAttachShader(id, fragment.id);
	GL_ERR();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
	GL_ERR();
}

void ShaderProgram::compile() {
	glLinkProgram(id);
	GL_ERR();
	glUseProgram(id);
	GL_ERR();
}

bool ShaderProgram::status() {
	return vertex.status() || fragment.status();
}
