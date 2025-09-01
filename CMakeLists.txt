cmake_minimum_required(VERSION 3.10)
project(Space)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source files
file(GLOB SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/**/*.h"
)

add_executable(Space ${SRC_FILES})

# ---------------------------
# GLFW
find_package(glfw3 REQUIRED)

# ---------------------------
# GLM & SPDLOG
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)

# ---------------------------
# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_include_directories(Space PRIVATE external/glad/include)

# ---------------------------
# IMGUI (vendored)
add_library(imgui
    external/imgui/imgui_demo.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)

# ---------------------------
# Link all libraries
target_link_libraries(Space
    glad
    glm
    spdlog
    glfw
    imgui
)

# ---------------------------
# On Linux, link OpenGL
if(UNIX)
    cmake_policy(SET CMP0072 NEW)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Space OpenGL::GL)
endif()
