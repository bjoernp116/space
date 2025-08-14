#include "renderer.h"
#include "input.h"
#include "spdlog/spdlog.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>

InputHandler::InputHandler() {
	for (int i = 0; i < 1024; i++) {
		keymap[i] = false;
	}
}

void InputHandler::key_callback(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
	Renderer *self = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	if (self) {
		/*if (ImGui::GetIO().WantCaptureKeyboard) {
		    spdlog::debug("ImGui want {0}", key);
		} else {
		    spdlog::debug("ImGui dont want {0}", key);
		}*/
		// spdlog::debug("{}", action);
		self->input_handler.keymap[key] = (action != GLFW_RELEASE);
	}
}

void InputHandler::mouse_callback(
    GLFWwindow *window, double xpos, double ypos) {
	Renderer *self = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	if (self) {
		self->input_handler.xpos = xpos;
		self->input_handler.ypos = ypos;
	}
}

void InputHandler::toggle_mouse_lock(GLFWwindow *window, ImGuiIO &io) {
	if (keymap[GLFW_KEY_F12]) {
		mouse_locked = false;
	}
	if (keymap[GLFW_KEY_F11]) {
		mouse_locked = true;
	}
	if (!mouse_locked) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void InputHandler::handle_mouse(Transform *camera) {
	if (!mouse_locked)
		return;
	float sensitivity = 0.0012f;

	if (first) {
		mouse_x = xpos;
		mouse_y = ypos;
		first = false;
	}

	float xoffset = mouse_x - xpos;
	float yoffset = mouse_y - ypos; // Reversed: y goes from bottom to top

	mouse_x = xpos;
	mouse_y = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera->rotation.y += xoffset;
	camera->rotation.x += yoffset;

	// Clamp pitch (x-axis rotation)
	camera->rotation.x = glm::clamp(camera->rotation.x,
	    -glm::half_pi<float>() + 0.01f,
	    glm::half_pi<float>() - 0.01f);
}

void Renderer::basic_camera_movement(float speed) {

	if (input_handler.keymap[GLFW_KEY_W]) {
		view.position += view.forward() * glm::vec3(speed);
	}
	if (input_handler.keymap[GLFW_KEY_S]) {
		view.position -= view.forward() * glm::vec3(speed);
	}
	if (input_handler.keymap[GLFW_KEY_A]) {
		view.position -= view.right() * glm::vec3(speed);
	}
	if (input_handler.keymap[GLFW_KEY_D]) {
		view.position += view.right() * glm::vec3(speed);
	}

	input_handler.handle_mouse(&view);
}
