#include <glad/glad.h>
#include "space.h"
#include "entity.h"
#include "input.h"
#include "mesh.h"
#include "transform.h"
#include "utils.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <glm/fwd.hpp>
#include <iostream>
#include <ostream>
#include <spdlog/spdlog.h>

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
		spdlog::error("Failed to create GLFW window\n");
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
		spdlog::error("Failed to initialize GLAD");
		return -1;
	}

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	spdlog::debug("OpenGL Version: {0}.{1}", major, minor);
	const unsigned char *glsl_version =
	    glGetString(GL_SHADING_LANGUAGE_VERSION);
	spdlog::debug("glsl: {0}", reinterpret_cast<const char *>(glsl_version));
	spdlog::debug("--------------------------");

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // ensures callback is called
	                                       // synchronously
	glDebugMessageCallback(debug_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE,
	    GL_DONT_CARE,
	    GL_DONT_CARE,
	    0,
	    nullptr,
	    GL_TRUE);

	Mesh pyramid("pyramid.obj");

	Transform transform(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.scale = glm::vec3(1.0f);

	Entity entity1(&pyramid, transform);

	transform.position.x = 1.5f;

	ShaderProgram shader_program("vertex.glsl", "fragment.glsl");
	shader_program.use();

	shader_program.declare({"view",
	    "projection",
	    "model",
	    "mat_diffuse",
	    "mat_specular",
	    "mat_shininess",
	    "light_size",
	    "view_pos"});

	Light light =
	    Light(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(1.0, 1.0, 0.9), 1.0f);

	Renderer renderer((float)width / (float)height);
	renderer.push(entity1);
	Light *light_ptr = renderer.push(light);
	float speed = 0.2;

	while (!glfwWindowShouldClose(window)) {
		renderer.clear();
		// glUseProgram(shader_program.id);
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

		// light_ptr->position = renderer.view.position;

		renderer.draw(shader_program);

		input_handler.handle_mouse(&renderer.view);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}

void Space::update() {}

Space::~Space() {}
