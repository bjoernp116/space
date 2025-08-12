#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Light {
  public:
	glm::vec3 position;
	glm::vec3 color;
	float intesity;
	Light(glm::vec3 position, glm::vec3 color, float intensity);
};
