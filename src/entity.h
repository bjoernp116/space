#pragma once
#include <glm/glm.hpp>

class Entity {
  public:
	glm::vec3 position, size, velocity, color;

	Entity();
	Entity(glm::vec3 pos,
	    glm::vec3 size,
	    glm::vec3 color = glm::vec3(1.0f),
	    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual void draw();
};
