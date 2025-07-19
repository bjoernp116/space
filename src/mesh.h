#pragma once

#include "buffer.h"

class Mesh {
  public:
	VertexBuffer vb;
	IndexBuffer ib;
	VertexArray vao;
	unsigned int index_count;

	Mesh(const void *v_data,
	    unsigned int v_size,
	    const unsigned int *i_data,
	    unsigned int i_count);
};
