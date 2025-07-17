#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "space.h"


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Space space(800, 600);
    space.init();
    while(!glfwWindowShouldClose(space.window)) {
        space.update();
        space.render();
    }

    glfwTerminate();
    return 0;
}

