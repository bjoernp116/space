#pragma once

#include "transform.h"
#include <GLFW/glfw3.h>
class InputHandler {
  public:
	bool keymap[1024];
	double mouse_x, mouse_y;
	double xpos, ypos;
	bool first;

	static void key_callback(
	    GLFWwindow *window, int key, int scancode, int action, int mods);

	void handle_mouse(Transform *camera);

	static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
};
