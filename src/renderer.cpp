#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "shader.h"
#include <imgui/imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

Renderer::Renderer(unsigned int width, unsigned int height, GLFWwindow *window)
    : view(), fbo() {
	view.position = glm::vec3(0.0f, 0.0f, 3.0f);
	view.rotation = glm::vec3(0.0f);
	view.scale = glm::vec3(1.0f);
	projection = glm::perspective(glm::radians(45.f),
	    (float)width / (float)height,
	    0.1f,
	    100.0f);
	frame_texture.set_size(width, height);
	fbo.attach_texture(frame_texture);
	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, resize_callback);

	ImGuiIO &io = ImGui::GetIO();
	default_font = io.Fonts->AddFontFromFileTTF(
	    "./fonts/Funnel_Sans/static/FunnelSans-Medium.ttf",
	    23.0f,
	    NULL,
	    io.Fonts->GetGlyphRangesDefault());
	title_font = io.Fonts->AddFontFromFileTTF(
	    "./fonts/Funnel_Sans/static/FunnelSans-Bold.ttf",
	    40.0f,
	    NULL,
	    io.Fonts->GetGlyphRangesDefault());
	IM_ASSERT(default_font != NULL);
	IM_ASSERT(title_font != NULL);
	gizmo_shader = new ShaderProgram("vertex.glsl", "wireframe_fragment.glsl");

	gizmo_shader->declare({
	    "view",
	    "projection",
	    "model",
	    "color",
	});
}

void Renderer::resize(unsigned int w, unsigned int h) {
	width = w;
	height = h;

	frame_texture = Texture();
	frame_texture.set_size(w, h);
	fbo.attach_texture(frame_texture);
}

void Renderer::resize_callback(GLFWwindow *window, int width, int height) {
	Renderer *renderer =
	    static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	renderer->resize(width, height);
}

void Renderer::clear() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const Entity &entity) const {
	ShaderProgram *shader_program = entity.mesh->shader_program;
	if (shader_program == nullptr) {
		spdlog::error("Mesh: {0} has no shader program!",
		    entity.mesh->class_name());
		return;
	}

	entity.mesh->bind_vao();
	shader_program->use();
	// spdlog::debug("{0} Drawn!", entity.name);

	glm::mat4 model = entity.transform.matrix();
	shader_program->set_matrix4("model", model);
	shader_program->set_matrix4("view", view.inverse_matrix());
	shader_program->set_matrix4("projection", projection);

	shader_program->set_vector3f("mat_diffuse", entity.material.diffuse);
	shader_program->set_vector3f("mat_specular", entity.material.specular);
	shader_program->set_float("mat_shininess", entity.material.shininess);
	shader_program->set_vector3f("view_pos", view.position);
	shader_program->set_integer("light_size", lights.size());
	shader_program->set_lights(lights);

	glDrawElements(GL_TRIANGLES,
	    entity.mesh->get_index_count(),
	    GL_UNSIGNED_INT,
	    0);
}

void Renderer::draw_debug(const Entity &entity) const {
	ShaderProgram *shader_program = const_cast<ShaderProgram *>(gizmo_shader);

	entity.mesh->bind_vao();
	shader_program->use();

	glm::mat4 model = entity.transform.matrix();
	shader_program->set_matrix4("model", model);
	shader_program->set_matrix4("view", view.inverse_matrix());
	shader_program->set_matrix4("projection", projection);
	shader_program->set_vector3f("color", glm::vec3(1.0f, 0.0f, 0.0f));

	glDrawElements(GL_LINES,
	    entity.mesh->get_index_count(),
	    GL_UNSIGNED_INT,
	    0);
}
void Renderer::draw() const {
	fbo.bind();
	glViewport(0, 0, frame_texture.get_size().x, frame_texture.get_size().y);
	clear();
	for (Entity entity : entities) {
		if (entity.visible)
			draw(entity);
		if (entity.debug || entity.mesh->class_name() == "Debug Mesh")
			draw_debug(entity);
	}
	fbo.unbind();
	ImGui::Begin("Viewport");
	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(intptr_t)frame_texture.id,
	    size,
	    ImVec2(0, 1),
	    ImVec2(1, 0));
	ImGui::End();
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
	ImGui::PushFont(title_font);
	ImGui::Text("Camera:");
	ImGui::Spacing();
	ImGui::PopFont();
	ImGui::Text("Position: (%.1f, %.1f, %.1f)",
	    view.position.x,
	    view.position.y,
	    view.position.z);
	ImGui::Text("Rotation: (%.1f, %.1f, %.1f)",
	    view.rotation.x,
	    view.rotation.y,
	    view.rotation.z);

	ImGui::Text("Scale: (%.1f, %.1f, %.1f)",
	    view.scale.x,
	    view.scale.y,
	    view.scale.z);
	ImGui::End();

	ImGui::Begin(entity_names[selected_entity]);
	ImGui::Text("Class: %s",
	    entities[selected_entity].mesh->class_name().c_str());
	ImGui::Text("Visible: ");
	ImGui::SameLine();
	ImGui::Checkbox("##visible", &entities[selected_entity].visible);
	ImGui::Text("Wireframe: ");
	ImGui::SameLine();
	ImGui::Checkbox("##wireframe", &entities[selected_entity].debug);
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
	if (entities[selected_entity].mesh->normal) {
		ImGui::Text("Normals: explicit");
	} else {
		ImGui::Text("Normals: implicit");
	}
	ImGui::End();
	ImGui::PopFont();
}
