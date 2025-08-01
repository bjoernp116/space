#include <cctype>
#include <iostream>
#include <spdlog/spdlog.h>
#include "buffer.h"
#include "utils.h"
#include "mesh.h"

Mesh::Mesh(const void *v_data,
    unsigned int v_size,
    const unsigned int *i_data,
    unsigned int i_count)
    : vb(v_data, v_size), ib(i_data, i_count), index_count(i_count) {

	VertexBufferLayout layout;
	layout.push<float>(3);
	vao.add_buffer(vb, layout);
}

const std::string MESH_PATH = "./meshes/";

Mesh::Mesh(const char *path) {
	std::string file = read_file(MESH_PATH + path);
	std::vector<float> vertecies;
	std::vector<unsigned int> indecies;
	int line = 0;
	for (std::string str : read_lines(file)) {
		line += 1;
		if (str.size() < 7 || str[0] == '#') {
			continue;
		}
		std::vector<std::string> column = split(str, ' ');
		if (!std::isalpha(column[0][0])) {
			spdlog::error("Parser error: line {0}: missing command, got {1}",
			    line,
			    column[0]);
			continue;
		}
		if (column[0][0] == 'v') {
			vertecies.push_back(std::stof(column[1]));
			vertecies.push_back(std::stof(column[2]));
			vertecies.push_back(std::stof(column[3]));
			continue;
		}
		if (column[0][0] == 'f') {
			indecies.push_back(std::stoi(column[1]) - 1);
			indecies.push_back(std::stoi(column[2]) - 1);
			indecies.push_back(std::stoi(column[3]) - 1);
			continue;
		}
	}
	vb.add_data(vertecies);
	ib.add_data(indecies);
	index_count = indecies.size();

	VertexBufferLayout layout;
	layout.push<float>(3);
	vao.add_buffer(vb, layout);
}

unsigned int Mesh::get_index_count() const {
	return index_count;
}

void Mesh::bind_vao() const {
	vao.bind();
}
