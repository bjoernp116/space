#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "space.h"
#include "entity.h"
#include "input.h"
#include "mesh.h"
#include "primitives.h"
#include "transform.h"
#include "ui.h"
#include "utils.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <glm/fwd.hpp>
#include <spdlog/spdlog.h>

/*void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}*/

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
	// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	float main_scale =
	    ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	ImGuiStyle &style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale * 1);
	style.FontScaleDpi = main_scale * 1;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	spdlog::debug("--------------------------");

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // ensures callback is called
	glDisable(GL_CULL_FACE);
	// synchronously
	glDebugMessageCallback(debug_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE,
	    GL_DONT_CARE,
	    GL_DONT_CARE,
	    0,
	    nullptr,
	    GL_TRUE);
	ShaderProgram shader_program("vertex.glsl", "fragment.glsl");
	shader_program.use();

	Renderer renderer(width, height, window);

	Box box;
	box.shader_program = &shader_program;
	// Mesh pyramid("cube.obj");
	// pyramid.shader_program = &shader_program;
	//  DebugMesh debug_box(&box);
	//  debug_box.shader_program = renderer.gizmo_shader;

	Transform transform(glm::vec3(0.0f));
	Entity box_entity("Box", &box, transform);

	transform.position.y += 0.5;
	// Entity pyramid_entity("Pyramid", &pyramid, transform);

	// Entity debug_entity("Debug Entity", &debug_box, transform);

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

	renderer.push(box_entity);
	// renderer.push(pyramid_entity);
	//  renderer.push(debug_entity);

	Light *light_ptr = renderer.push(light);
	float speed = 0.2;

	while (!glfwWindowShouldClose(window)) {
		// Get framebuffer size instead of using io.DisplaySize
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		renderer.input_handler.toggle_mouse_lock(window, io);

		renderer.draw_ui(io);
		ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
		ImVec2 viewport_pos = ImGui::GetWindowPos();

		/*glViewport(viewport_pos.x,
		    display_h - (int)viewport_pos.y - (int)viewport_panel_size.y,
		    (int)viewport_panel_size.x,
		    (int)viewport_panel_size.y);
		    */

		// Clear only viewport area before drawing scene

		// Update camera movement here if you want
		renderer.basic_camera_movement(speed);

		renderer.draw();

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void Space::update() {}

Space::~Space() {}
