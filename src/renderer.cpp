#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

Renderer::Renderer(const float aspect_ratio) : view() {
	view.position = glm::vec3(0.0f, 0.0f, 3.0f);
	view.rotation = glm::vec3(0.0f);
	view.scale = glm::vec3(1.0f);
	projection =
	    glm::perspective(glm::radians(45.f), aspect_ratio, 0.1f, 100.0f);
}

void Renderer::clear() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const Mesh &mesh,
    const ShaderProgram &shader_program) const {
	mesh.bind_vao();
	shader_program.use();
	float time = glfwGetTime();
	glm::mat4 model =
	    glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 1.0f));
	shader_program.set_matrix4("u_Model", model);
	shader_program.set_matrix4("u_View", view.matrix());
	shader_program.set_matrix4("u_Projection", projection);

	glDrawElements(GL_TRIANGLES, mesh.get_index_count(), GL_UNSIGNED_INT, 0);
}

void Renderer::draw(const Entity &entity,
    const ShaderProgram &shader_program) const {

	entity.mesh->bind_vao();
	shader_program.use();
	glm::mat4 model = entity.transform.matrix();
	shader_program.set_matrix4("u_Model", model);
	shader_program.set_matrix4("u_View", view.inverse_matrix());
	shader_program.set_matrix4("u_Projection", projection);
	int vao = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
	spdlog::debug("VAO: Binding: {}", vao);

	glDrawElements(GL_TRIANGLES,
	    entity.mesh->get_index_count(),
	    GL_UNSIGNED_INT,
	    0);
}

void Renderer::draw(const ShaderProgram &shader_program) const {
	for (Entity entity : entities) {
		draw(entity, shader_program);
	}
}

void Renderer::push(const Entity &entity) {
	entities.push_back(entity);
}
