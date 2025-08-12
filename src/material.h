#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Material {
  public:
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Material();
	Material(glm::vec3 diffuse);
};
