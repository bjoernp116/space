#include <cassert>
#include <cctype>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include "buffer.h"
#include "glm/geometric.hpp"
#include "utils.h"
#include "mesh.h"
#include "resource_handler/obj.h"

const std::string Mesh::class_name() const {
	return std::string("Mesh");
}

Mesh::Mesh(const void *v_data,
    unsigned int v_size,
    const unsigned int *i_data,
    unsigned int i_count)
    : vb(v_data, v_size), ib(i_data, i_count), index_count(i_count) {

	VertexBufferLayout layout;
	layout.push<float>(3);
	vao.add_buffer(vb, layout);
	vao.add_buffer(ib);
}

const std::string MESH_PATH = "./meshes/";

std::vector<float> intertwine_buffers(const std::vector<float> v1,
    const unsigned int v1_size,
    const std::vector<float> v2,
    const unsigned int v2_size) {
	const float v1_ratio = (float)v1.size() / (float)v1_size;
	const float v2_ratio = (float)v2.size() / (float)v2_size;
	if (v1_ratio != v2_ratio) {
		spdlog::error("Missaligned buffer intertwining: v1 = {0} / {1} = {2}, "
		              "v2 = {3} / {4} = {5}",
		    v1.size(),
		    v1_size,
		    v1_ratio,
		    v2.size(),
		    v2_size,
		    v2_ratio);
	}
	std::vector<float> result;
	result.reserve(v1.size());

	for (int i = 0; i < v1.size() / v1_size; i += v1_size) {
		for (int j = 0; j < v1_size; j++) {
			result.push_back(v1.at(i + j));
			spdlog::debug("v1: {}", v1.at(i + j));
		}
		for (int j = 0; j < v2_size; j++) {
			result.push_back(v2.at(i + j));
			spdlog::debug("v2: {}", v1.at(i + j));
		}
	}
	return result;
}

std::vector<float> compute_flat_normals(std::vector<float> vertecies,
    const std::vector<unsigned int> &faces) {
	std::vector<float> normals(vertecies.size());
	for (int i = 0; i < faces.size(); i += 3) {
		glm::vec3 p1 = glm::vec3(vertecies[faces[i + 0] + 0],
		    vertecies[faces[i + 0] + 1],
		    vertecies[faces[i + 0] + 2]);
		glm::vec3 p2 = glm::vec3(vertecies[faces[i + 1] + 0],
		    vertecies[faces[i + 1] + 1],
		    vertecies[faces[i + 1] + 2]);
		glm::vec3 p3 = glm::vec3(vertecies[faces[i + 2] + 0],
		    vertecies[faces[i + 2] + 1],
		    vertecies[faces[i + 2] + 2]);

		/*spdlog::debug("p1: ({0}, {1}, {2})", p1.x, p1.y, p1.z);
		spdlog::debug("p2: ({0}, {1}, {2})", p2.x, p2.y, p2.z);
		spdlog::debug("p3: ({0}, {1}, {2})", p3.x, p3.y, p3.z);
		*/

		glm::vec3 edge1 = p2 - p1;
		glm::vec3 edge2 = p3 - p1;

		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		// spdlog::debug("normal: ({0}, {1}, {2})", normal.x, normal.y,
		// normal.z);

		normals[faces[i + 0] + 0] = normal.x;
		normals[faces[i + 0] + 1] = normal.y;
		normals[faces[i + 0] + 2] = normal.z;
		normals[faces[i + 1] + 0] = normal.x;
		normals[faces[i + 1] + 1] = normal.y;
		normals[faces[i + 1] + 2] = normal.z;
		normals[faces[i + 2] + 0] = normal.x;
		normals[faces[i + 2] + 1] = normal.y;
		normals[faces[i + 2] + 2] = normal.z;
	}
	return normals;
}

Mesh::Mesh(ObjectFile objfile) {
	std::vector<float> vertecies = objfile.get_vertecies<float>();
	std::vector<float> normals = objfile.get_normals<float>();
	std::vector<unsigned int> indecies = objfile.get_indecies<unsigned int>();

	if (vertecies.size() == normals.size()) {
		spdlog::debug("Using normals");
	} else {
		spdlog::warn("Auto computing normals: vertecies: {0}, normals: {1}",
		    vertecies.size(),
		    normals.size());
		spdlog::warn("Normal generator is not yet implemented!");
	}
	VertexBufferLayout layout;
	std::vector<float> vertnorm = intertwine_buffers(vertecies, 3, normals, 3);

	for (float n : vertnorm) {
		spdlog::debug("Vertnorm: {0}", n);
	}
	for (unsigned int index : indecies) {
		spdlog::debug("{}", index);
	}

	layout.push<float>(3);
	layout.push<float>(3);
	// for (float n : vertnorm)
	//	spdlog::debug("{0}", n);

	vb.add_data(vertnorm);
	ib.add_data(indecies);
	index_count = indecies.size();

	vao.add_buffer(vb, layout);
	vao.add_buffer(ib);
}

unsigned int Mesh::get_index_count() const {
	return index_count;
}

void Mesh::bind_vao() const {
	vao.bind();
}

Mesh::Mesh(const std::vector<float> vertecies,
    const std::vector<unsigned int> indecies,
    bool normal)
    : normal(normal) {

	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	vb.add_data(vertecies);
	ib.add_data(indecies);
	index_count = indecies.size();

	vao.add_buffer(vb, layout);
	vao.add_buffer(ib);
}

Mesh::Mesh(const std::vector<glm::vec3> vertecies,
    const std::vector<glm::uvec3> indecies,
    bool normal)
    : normal(normal) {

	std::vector<glm::vec3> out;
	out.reserve(vertecies.size() * 2);
	if (!normal) {
		for (glm::vec3 vertex : vertecies) {
			out.push_back(vertex);
			out.push_back(glm::vec3(0.0f));
		}
	} else {
		out = vertecies;
	}

	for (const auto &vertex : out) {
		spdlog::debug("Output vertex: ({0}, {1}, {2})",
		    vertex.x,
		    vertex.y,
		    vertex.z);
	}

	// std::vector<float> normals = compute_flat_normals(vertecies, indecies);
	// std::vector<float> vertnorm = intertwine_buffers(vertecies, normals);

	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	vb.add_data(out);
	ib.add_data(indecies);
	index_count = indecies.size() * 3;

	vao.add_buffer(vb, layout);
	vao.add_buffer(ib);
}

Mesh::Mesh(const std::vector<glm::vec3> vertecies,
    const std::vector<glm::uvec2> indecies,
    bool normal)
    : normal(normal) {
	std::vector<glm::vec3> out(vertecies.size() * 2);
	if (!normal) {
		for (glm::vec3 vertex : vertecies) {
			out.push_back(vertex);
			out.push_back(glm::vec3(0.0f));
		}
	} else {
		out = vertecies;
	}
	// std::vector<float> normals = compute_flat_normals(vertecies, indecies);
	// std::vector<float> vertnorm = intertwine_buffers(vertecies, normals);

	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	vb.add_data(vertecies);
	ib.add_data(indecies);
	index_count = indecies.size() * 2;

	vao.add_buffer(vb, layout);
	vao.add_buffer(ib);
}

std::vector<glm::vec3> Mesh::get_vertecies() const {
	int size = 0;
	glad_glGetNamedBufferParameteriv(vb.id, GL_BUFFER_SIZE, &size);
	if (sizeof(glm::vec3) != 3 * sizeof(float)) {
		spdlog::error("Vec3 is padded to size {0}", sizeof(glm::vec3));
	}
	std::vector<glm::vec3> vertecies(size / sizeof(glm::vec3));
	glGetNamedBufferSubData(vb.id, 0, size, vertecies.data());
	return vertecies;
}

std::vector<glm::uvec3> Mesh::get_indecies() const {
	int size;
	glad_glGetNamedBufferParameteriv(ib.id, GL_BUFFER_SIZE, &size);
	if (sizeof(glm::uvec3) != 3 * sizeof(unsigned int)) {
		spdlog::error("UVec3 is padded to size {0}", sizeof(glm::uvec3));
	}
	std::vector<glm::uvec3> indecies(size / sizeof(glm::uvec3));
	glGetNamedBufferSubData(ib.id, 0, size, indecies.data());
	return indecies;
}
