#include "resource_manager.h"
#include "shader.h"
#include <cassert>

std::map<std::string, ShaderProgram> ResourceManager::shaders;
std::map<std::string, Mesh> ResourceManager::meshes;

void ResourceManager::load_shader_program(
    std::string name, std::string vertex_path, std::string fragment_path) {
	ShaderProgram shader_program(vertex_path, fragment_path);

	shader_program.use();

	shaders.insert(std::make_pair(std::move(name), std::move(shader_program)));
}

ShaderProgram &ResourceManager::get_shader_program(std::string name) {
	return shaders.at(name);
}

void ResourceManager::load_mesh(std::string name, Mesh mesh) {
	meshes.insert(std::make_pair(std::move(name), std::move(mesh)));
}

Mesh &ResourceManager::get_mesh(std::string name) {
	return meshes.at(name);
}
