#include "buffer.h"
#include "mesh.h"

Mesh::Mesh(const void *v_data,
    unsigned int v_size,
    const unsigned int *i_data,
    unsigned int i_count)
    : vb(v_data, v_size), ib(i_data, i_count), index_count(i_count) {

	VertexBufferLayout layout;
	layout.push<float>(3);
	vao.add_buffer(vb, layout);

	vb.bind();
	ib.bind();
}

void Mesh::bind() const {
	vao.bind();
}

void Mesh::unbind() const {
	vao.unbind();
}

unsigned int Mesh::get_index_count() const {
	return index_count;
}
