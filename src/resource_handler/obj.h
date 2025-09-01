#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "file.h"
class ObjectFile : public File {

	std::vector<glm::vec3> vertecies;
	std::vector<glm::vec3> normals;
	std::vector<glm::uvec3> indecies;
	unsigned int face_size;

  public:
	ObjectFile(const char* path);

	template <typename T> std::vector<T> get_vertecies() const;
	template <typename T> std::vector<T> get_normals() const;
	template <typename T> std::vector<T> get_indecies() const;
	unsigned int get_face_size() const;
};
