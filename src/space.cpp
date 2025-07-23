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
#include <ostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void GLAPIENTRY debug_callback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "Source: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "Source: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "Source: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "Source: Other";
		break;
	}
	std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "Type: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "Type: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "Type: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "Type: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "Type: Other";
		break;
	}
	std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "Severity: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Severity: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Severity: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Severity: notification";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
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

	std::cout << GL_KHR_debug << std::endl;
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

	Mesh pyramid("pyramid.obj");

	Transform transform(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.scale = glm::vec3(1.0f);

	Entity entity1(&pyramid, transform);

	transform.position.x = 1.5f;

	ShaderProgram shader_program("vertex.glsl", "fragment.glsl");
	shader_program.use();

	shader_program.declare({"u_View", "u_Projection", "u_Model"});

	Renderer renderer((float)width / (float)height);
	renderer.push(entity1);

	float speed = 0.2;

	while (!glfwWindowShouldClose(window)) {
		renderer.clear();
		// glUseProgram(shader_program.id);
		std::cout << "." << std::flush;
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

		renderer.draw(shader_program);

		input_handler.handle_mouse(&renderer.view);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}

void Space::update() {}

Space::~Space() {}
