cmake_minimum_required(VERSION 3.10)
project(Space)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

# Add source files
file(GLOB SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(Space ${SRC_FILES})

# Include GLFW
find_package(glfw3 REQUIRED)

# GLAD

#add_subdirectory(external/glad)
#target_link_libraries(Space glad)

add_subdirectory(external/glm)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(Space glad glm)
target_include_directories(Space PRIVATE external/glad/include)

# Link GLFW
target_link_libraries(Space glfw)


# On Linux, link OpenGL
if(UNIX)
    cmake_policy(SET CMP0072 NEW)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Space OpenGL::GL)
endif()
