#include "resource_manager.h"
#include "buffer.h"
#include "shader.h"
#include <cassert>

std::map<std::string, ShaderProgram> ResourceManager::shaders;

void ResourceManager::load_shader_program(
    std::string name, std::string vertex_path, std::string fragment_path) {
	ShaderProgram shader_program(vertex_path, fragment_path);

	shader_program.compile();
	assert(shader_program.status());

	shaders.insert(std::make_pair(std::move(name), std::move(shader_program)));
}

ShaderProgram &ResourceManager::get_shader_program(std::string name) {
	return shaders.at(name);
}
/*
void ResourceManager::load_index_buffer(std::string name,
    IndexBuffer &&index_buffer) {
    index_buffers.insert(
        std::make_pair(std::move(name), std::move(index_buffer)));
}

IndexBuffer &ResourceManager::get_index_buffer(std::string name) {
    return index_buffers.at(name);
}

void ResourceManager::load_vertex_buffer(std::string name,
    VertexBuffer &&vertex_buffer) {
    vertex_buffers.insert(
        std::make_pair(std::move(name), std::move(vertex_buffer)));
}

VertexBuffer &ResourceManager::get_vertex_buffer(std::string name) {
    return vertex_buffers.at(name);
}

void ResourceManager::set_vertex_array(VertexArray vao) {
    vertex_array = vao;
}

VertexArray &ResourceManager::get_vertex_array() {
    return vertex_array;
}
*/
