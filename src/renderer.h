#pragma once

#include "entity.h"
#include "imgui.h"
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
	void draw_ui(ImGuiIO &io);
	void push(const Entity &entity);
	Light *push(const Light &light);

  private:
	std::vector<Entity> entities;
	std::vector<Light> lights;

	// UI
	int selected_entity;
	std::vector<const char *> entity_names;
};
