#pragma once

#include "entity.h"
#include "shader.h"
#include <glm/fwd.hpp>

class Renderer {
  public:
	std::vector<Entity> entities;

	Transform view;

	glm::mat4 projection;

	Renderer(const float aspect_ratio);
	void clear() const;
	void draw(const Mesh &mesh, const ShaderProgram &shader_program) const;
	void draw(const Entity &entity, const ShaderProgram &shader_program) const;
};
