#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include <string>
#include <vector>
#include "light.h"

class Shader {
  public:
	unsigned int id;
	std::string src;
	int type;

	Shader(std::string path, GLenum shader_type);
	~Shader();
	void compile_shader() const;
	bool status() const;
};
class ShaderProgram {
  public:
	unsigned int id;
	std::map<std::string, int> location_table;

	ShaderProgram(std::string vertex_path, std::string fragment_path);
	~ShaderProgram();

	void declare(std::vector<const char *> identifiers);

	void use() const;

	void set_float(const char *name, float value) const;
	void set_integer(const char *name, int value) const;
	void set_vector2f(const char *name, float x, float y) const;
	void set_vector2f(const char *name, const glm::vec2 &value) const;
	void set_vector3f(const char *name, float x, float y, float z) const;
	void set_vector3f(const char *name, const glm::vec3 &value) const;
	void set_vector4f(
	    const char *name, float x, float y, float z, float w) const;
	void set_vector4f(const char *name, const glm::vec4 &value) const;
	void set_matrix4(const char *name, const glm::mat4 &matrix) const;

	void set_lights(const std::vector<Light> light) const;
};
