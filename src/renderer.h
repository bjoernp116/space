#pragma once

#include "buffer.h"
#include "entity.h"
#include "imgui.h"
#include "input.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

class Renderer {
  public:
	Transform view;
	InputHandler input_handler;

	glm::mat4 projection;

	Renderer(unsigned int width, unsigned int height, GLFWwindow *window);
	static void clear();
	void resize(unsigned int width, unsigned int height);
	void draw(const Entity &entity) const;
	void draw_debug(const Entity &entity) const;
	void draw() const;
	void draw_ui(ImGuiIO &io);
	void push(const Entity &entity);
	Light *push(const Light &light);

	void basic_camera_movement(float speed);

	ShaderProgram *gizmo_shader;

  private:
	FrameBuffer fbo;
	Texture frame_texture;
	static void resize_callback(GLFWwindow *window, int width, int height);
	unsigned int width;
	unsigned int height;

	ImFont *title_font;
	ImFont *default_font;

	std::vector<Entity> entities;
	std::vector<Light> lights;

	// UI
	int selected_entity = 0;
	std::vector<const char *> entity_names;
};
