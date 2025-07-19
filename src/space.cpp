#include <glad/glad.h>
#include "space.h"
#include "shader.h"
#include "buffer.h"
#include "utils.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <cassert>
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
        // Position (x, y, z)
        -0.5f, -0.5f, -0.5f, // 0
        0.5f, -0.5f, -0.5f, // 1
        0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3
        -0.5f, -0.5f,  0.5f, // 4
        0.5f, -0.5f,  0.5f, // 5
        0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };
	unsigned int indices[] = {
        // back face
        0, 1, 2,
        2, 3, 0,
        // front face
        4, 5, 6,
        6, 7, 4,
        // left face
        4, 0, 3,
        3, 7, 4,
        // right face
        1, 5, 6,
        6, 2, 1,
        // bottom face
        4, 5, 1,
        1, 0, 4,
        // top face
        3, 2, 6,
        6, 7, 3
    };

    VertexArray VAO;

    VertexBuffer VBO(vertices, 12 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(3);

    VAO.add_buffer(VBO, layout);

    VBO.bind();

    IndexBuffer EBO(indices, 6);
    EBO.bind();

    ShaderProgram shader_program("vertex.glsl", "fragment.glsl");
    shader_program.compile();
    shader_program.status();

    VAO.unbind();
    VBO.unbind();
    EBO.unbind();

    Renderer renderer;
    while (!glfwWindowShouldClose(window)) {
        renderer.clear();

        // glUseProgram(shader_program.id);

        renderer.draw(VAO, EBO, shader_program);

        glfwSwapBuffers(window);
        GL_ERR();
        glfwPollEvents();
        GL_ERR();
    }
    return 0;
}

void Space::render() {
    return;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GL_ERR();
    glClear(GL_COLOR_BUFFER_BIT);
    GL_ERR();

    ShaderProgram &shader_program = resource_manager.get_shader_program("main");
    shader_program.compile();
    shader_program.status();
    glUseProgram(shader_program.id);

    VertexBuffer &VBO = resource_manager.get_vertex_buffer("vbo");
    IndexBuffer &EBO = resource_manager.get_index_buffer("ebo");

    glDrawElements(GL_TRIANGLES,
            resource_manager.get_index_buffer("ebo").count,
            GL_UNSIGNED_INT,
            0);
    GL_ERR();

    glBindVertexArray(0);
    GL_ERR();

    glfwSwapBuffers(window);
    GL_ERR();
    glfwPollEvents();
    GL_ERR();
}

void Space::update() {}

Space::~Space() {}
