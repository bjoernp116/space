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
	void process_input();
	void update();
	void render();
};
