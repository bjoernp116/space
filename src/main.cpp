#include "space.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

int main() {
	spdlog::set_level(spdlog::level::debug);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Space space(800, 600);
	space.init();
	/*while (!glfwWindowShouldClose(space.window)) {
	    space.update();
	    space.render();
	}*/

	glfwTerminate();
	return 0;
}
