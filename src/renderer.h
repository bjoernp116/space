#pragma once

#include "buffer.h"
#include "shader.h"

class Renderer {
  public:
	Renderer();
	void clear() const;
	void draw(const VertexArray &VAO,
	    const IndexBuffer &EBO,
	    const ShaderProgram &shader_program) const;
};
