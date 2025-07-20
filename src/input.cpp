#include "input.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>

void InputHandler::key_callback(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
	InputHandler *self =
	    static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
	if (self) {
		self->keymap[key] = (action != GLFW_RELEASE);
	}
}

void InputHandler::mouse_callback(
    GLFWwindow *window, double xpos, double ypos) {
	InputHandler *self =
	    static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
	if (self) {
		self->xpos = xpos;
		self->ypos = ypos;
	}
}

void InputHandler::handle_mouse(Transform *camera) {
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
