#pragma once

#include "entity.h"
#include "shader.h"
#include <glm/fwd.hpp>

class Renderer {
  public:
	Transform view;

	glm::mat4 projection;

	Renderer(const float aspect_ratio);
	static void clear();
	void draw(const Mesh &mesh, const ShaderProgram &shader_program) const;
	void draw(const Entity &entity, const ShaderProgram &shader_program) const;
	void draw(const ShaderProgram &shader_program) const;
	void push(const Entity &entity);

  private:
	std::vector<Entity> entities;
};
