#include <spdlog/spdlog.h>
#include "shader.h"
#include "utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <string>

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
		if (location == -1) {
			spdlog::error("Cant declare: Uniform {0} does not exist",
			    identifier);
			continue;
		}
		location_table.insert(std::make_pair(identifier, location));
	}
}

void ShaderProgram::set_matrix4(const char *name,
    const glm::mat4 &matrix) const {
	if (location_table.find(name) != location_table.end()) {
		int location = location_table.at(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}
}

void ShaderProgram::set_float(const char *name, float value) const {
	if (location_table.find(name) != location_table.end()) {
		int location = location_table.at(name);
		glUniform1f(location, value);
	}
}

void ShaderProgram::set_integer(const char *name, int value) const {
	if (location_table.find(name) != location_table.end()) {
		int location = location_table.at(name);
		glUniform1i(location, value);
	}
}

void ShaderProgram::set_vector3f(
    const char *name, float x, float y, float z) const {
	if (location_table.find(name) != location_table.end()) {
		int location = location_table.at(name);
		glUniform3f(location, x, y, z);
	}
}

void ShaderProgram::set_vector3f(const char *name,
    const glm::vec3 &value) const {
	if (location_table.find(name) != location_table.end()) {
		int location = location_table.at(name);
		glUniform3f(location, value.x, value.y, value.z);
	}
}

void ShaderProgram::set_lights(const std::vector<Light> lights) const {
	for (int i = 0; i < lights.size(); i++) {
		std::string basename = "lights[" + std::to_string(i) + "]";
		int pos_loc =
		    glGetUniformLocation(id, (basename + ".position").c_str());
		int col_loc = glGetUniformLocation(id, (basename + ".color").c_str());
		int int_loc =
		    glGetUniformLocation(id, (basename + ".intensity").c_str());
		glUniform3fv(pos_loc, 1, glm::value_ptr(lights[i].position));
		glUniform3fv(col_loc, 1, glm::value_ptr(lights[i].color));
		glUniform1f(int_loc, lights[i].intesity);
	}
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
