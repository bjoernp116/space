#pragma once

#include "buffer.h"
#include "shader.h"

class Mesh {
  public:
	Mesh(const void *v_data,
	    unsigned int v_size,
	    const unsigned int *i_data,
	    unsigned int i_count);
	Mesh(const char *path);
	void draw(const ShaderProgram &shader_program) const;
	unsigned int get_index_count() const;
	void bind_vao() const;

  private:
	VertexBuffer vb;
	IndexBuffer ib;
	VertexArray vao;
	unsigned int index_count;
};
