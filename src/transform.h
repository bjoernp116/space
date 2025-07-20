#pragma once

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class Transform {
  public:
	glm::vec3 position, rotation, scale;

	Transform();
	Transform(glm::vec3 position);

	glm::mat4 matrix() const;
	glm::mat4 inverse_matrix() const;

	glm::vec3 transform_axis(glm::vec3 axi) const;

	glm::vec3 forward() const;
	glm::vec3 right() const;
};
