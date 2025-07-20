#include "transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

Transform::Transform() {}

Transform::Transform(glm::vec3 pos) : position(pos) {}

glm::mat4 Transform::matrix() const {
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

	glm::mat4 rotx =
	    glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
	glm::mat4 roty =
	    glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 rotz =
	    glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(1, 0, 1));
	glm::mat4 rotation = rotz * roty * rotx;

	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

	return translation * rotation * scaling;
}

glm::mat4 Transform::inverse_matrix() const {
	return glm::inverse(matrix());
}

glm::vec3 Transform::forward() const {
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	return transform_axis(forward);
}

glm::vec3 Transform::right() const {
	glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f);
	return transform_axis(forward);
}

glm::vec3 Transform::transform_axis(glm::vec3 axis) const {
	// Assuming Y-up, Z-forward (OpenGL convention)
	// Create a rotation matrix
	glm::mat4 rotx =
	    glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
	glm::mat4 roty =
	    glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 rotz =
	    glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 rotation_matrix = rotz * roty * rotx;

	// Apply rotation to forward vector
	return glm::vec3(rotation_matrix * glm::vec4(axis, 0.0f));
}
