#pragma once

#include "shader.h"
#include <GLFW/glfw3.h>
#include "resource_manager.h"

class Space {
  public:
	// GameState State;
	bool keymap[1024];
	unsigned int width, height;
	GLFWwindow *window;
	ResourceManager resource_manager;

	Space(unsigned int width, unsigned int height);
	~Space();

	int init();
	static void process_input(
	    GLFWwindow *window, int key, int scancode, int action, int mods);
	void update();
	void render();
};
