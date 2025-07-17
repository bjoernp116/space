#include "resource_manager.h"
#include "shader.h"
#include <cassert>

std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::load_shader(
    std::string name,
    const char* path,
    GLenum shader_type
) {
    Shader shader(path, shader_type);    
    
    shader.compile_shader();
    assert(shader.status());

    shaders.insert(std::make_pair(name, shader));
    return shader;
}

Shader ResourceManager::get_shader(std::string name) {
    return shaders.at(name);
}
