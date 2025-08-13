#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include <imgui/imgui.h>
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	shader_program.set_matrix4("model", model);
	shader_program.set_matrix4("view", view.inverse_matrix());
	shader_program.set_matrix4("projection", projection);

	shader_program.set_vector3f("mat_diffuse", entity.material.diffuse);
	shader_program.set_vector3f("mat_specular", entity.material.specular);
	shader_program.set_float("mat_shininess", entity.material.shininess);
	shader_program.set_vector3f("view_pos", view.position);
	shader_program.set_integer("light_size", lights.size());
	shader_program.set_lights(lights);

	int vao = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);

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
	entity_names.push_back(entity.name);
}

Light *Renderer::push(const Light &light) {
	lights.push_back(light);
	return &lights.back();
}

void Renderer::draw_ui(ImGuiIO &io) {
	ImFont *default_font = io.Fonts->AddFontFromFileTTF(
	    "./fonts/Funnel_Sans/static/FunnelSans-Medium.ttf",
	    23.0f,
	    NULL,
	    io.Fonts->GetGlyphRangesDefault());
	ImFont *title_font = io.Fonts->AddFontFromFileTTF(
	    "./fonts/Funnel_Sans/static/FunnelSans-Bold.ttf",
	    40.0f,
	    NULL,
	    io.Fonts->GetGlyphRangesDefault());
	IM_ASSERT(default_font != NULL);
	IM_ASSERT(title_font != NULL);

	ImGui::PushFont(default_font);
	// Create a fullscreen window for docking space
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
	ImGuiWindowFlags window_flags =
	    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
	    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	ImGui::PopStyleVar(2);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f)); // Full size DockSpace

	ImGui::End();
	ImGui::Begin("Object Manager");

	ImGui::PushFont(title_font);
	ImGui::Text("Entities:");
	ImGui::Spacing();
	ImGui::PopFont();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::ListBox("##entitybox",
	    &selected_entity,
	    (const char *const *)entity_names.data(),
	    entity_names.size());
	ImGui::End();

	ImGui::Begin(entity_names[selected_entity]);
	ImGui::Text("Class: %s",
	    entities[selected_entity].mesh->class_name().c_str());
	ImGui::Text("Position: (%.1f, %.1f, %.1f)",
	    entities[selected_entity].transform.position.x,
	    entities[selected_entity].transform.position.y,
	    entities[selected_entity].transform.position.z);
	ImGui::Text("Rotation: (%.1f, %.1f, %.1f)",
	    entities[selected_entity].transform.rotation.x,
	    entities[selected_entity].transform.rotation.y,
	    entities[selected_entity].transform.rotation.z);
	ImGui::Text("Scale: (%.1f, %.1f, %.1f)",
	    entities[selected_entity].transform.scale.x,
	    entities[selected_entity].transform.scale.y,
	    entities[selected_entity].transform.scale.z);
	ImGui::End();
	ImGui::PopFont();
}
