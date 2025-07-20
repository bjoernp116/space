#include <glad/glad.h>
#include "space.h"
#include "entity.h"
#include "input.h"
#include "mesh.h"
#include "shader.h"
#include "transform.h"
#include "utils.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <glm/fwd.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

Space::Space(unsigned int w, unsigned int h) {
	width = w;
	height = h;
	window = glfwCreateWindow(width, height, "SPACE", nullptr, nullptr);
	resource_manager = ResourceManager();
}

int Space::init() {

	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	InputHandler input_handler;
	glfwSetWindowUserPointer(window, &input_handler);
	glfwSetKeyCallback(window, InputHandler::key_callback);

	glfwSetCursorPosCallback(window, InputHandler::mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "OpenGL Version: " << major << "." << minor << std::endl;

	std::cout << "glsl: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
	          << std::endl;
	std::cout << "--------------------------" << std::endl;

	float vertices[] = {
	    0.5,
	    0.5,
	    0.5,
	    -0.5,
	    0.5,
	    0.5,
	    -0.5,
	    -0.5,
	    0.5,
	    0.5,
	    -0.5,
	    0.5,

	    // Back face
	    0.5,
	    0.5,
	    -0.5,
	    -0.5,
	    0.5,
	    -0.5,
	    -0.5,
	    -0.5,
	    -0.5,
	    0.5,
	    -0.5,
	    -0.5,
	};
	unsigned int indices[] = {
	    // Front
	    0,
	    1,
	    2,
	    2,
	    3,
	    0,

	    // Right
	    0,
	    3,
	    7,
	    7,
	    4,
	    0,

	    // Bottom
	    2,
	    6,
	    7,
	    7,
	    3,
	    2,

	    // Left
	    1,
	    5,
	    6,
	    6,
	    2,
	    1,

	    // Back
	    4,
	    7,
	    6,
	    6,
	    5,
	    4,

	    // Top
	    5,
	    1,
	    0,
	    0,
	    4,
	    5,
	};

	Mesh cube(vertices,
	    sizeof(vertices),
	    indices,
	    sizeof(indices) / sizeof(unsigned int));
	Transform transform(glm::vec3(0.0f, 1.0f, 0.0f));
	transform.rotation = glm::vec3(0.0f, 0.5f, 0.0f);
	transform.scale = glm::vec3(1.0f);

	Entity entity(&cube, transform);

	ShaderProgram shader_program("vertex.glsl", "fragment.glsl");
	shader_program.use();

	shader_program.declare({"u_View", "u_Projection", "u_Model"});

	Renderer renderer((float)width / (float)height);
	float speed = 0.2;

	while (!glfwWindowShouldClose(window)) {
		renderer.clear();
		// glUseProgram(shader_program.id);
		//
		if (input_handler.keymap[GLFW_KEY_W]) {
			renderer.view.position +=
			    renderer.view.forward() * glm::vec3(speed);
		}
		if (input_handler.keymap[GLFW_KEY_S]) {
			renderer.view.position -=
			    renderer.view.forward() * glm::vec3(speed);
		}
		if (input_handler.keymap[GLFW_KEY_A]) {
			renderer.view.position -= renderer.view.right() * glm::vec3(speed);
		}
		if (input_handler.keymap[GLFW_KEY_D]) {
			renderer.view.position += renderer.view.right() * glm::vec3(speed);
		}

		renderer.draw(entity, shader_program);

		input_handler.handle_mouse(&renderer.view);

		glfwSwapBuffers(window);
		GL_ERR();
		glfwPollEvents();
		GL_ERR();
	}
	return 0;
}

void Space::update() {}

Space::~Space() {}
