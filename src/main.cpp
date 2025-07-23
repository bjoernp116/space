#include "space.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

int main() {
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
