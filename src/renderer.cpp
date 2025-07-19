#include <glad/glad.h>
#include "renderer.h"
#include "utils.h"
#include <iostream>

Renderer::Renderer() {}

void Renderer::clear() const {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GL_ERR();
	glClear(GL_COLOR_BUFFER_BIT);
	GL_ERR();
}

void Renderer::draw(const VertexArray &VAO,
    const IndexBuffer &EBO,
    const ShaderProgram &shader_program) const {

	VAO.bind();
	EBO.bind();

	glDrawElements(GL_TRIANGLES, EBO.count, GL_UNSIGNED_INT, 0);
	GL_ERR();
}
