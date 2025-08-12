#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include "material.h"

Material::Material() {
	diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	specular = glm::vec3(0.5f, 0.5f, 0.5f);
	shininess = 32.0f;
}
Material::Material(glm::vec3 diffuse) : diffuse(diffuse) {
	specular = glm::vec3(0.3f, 0.3f, 0.3f);
	shininess = 32.0f;
}
