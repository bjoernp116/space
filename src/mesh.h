#pragma once

#include "buffer.h"
#include "shader.h"

class Mesh {
  public:
	Mesh(const void *v_data,
	    unsigned int v_size,
	    const unsigned int *i_data,
	    unsigned int i_count);

	Mesh(const std::vector<float> vertecies,
	    const std::vector<unsigned int> indecies,
	    bool normal);

	Mesh(const std::vector<glm::vec3> vertecies,
	    const std::vector<glm::uvec3> indecies,
	    bool normal);

	Mesh(const std::vector<glm::vec3> vertecies,
	    const std::vector<glm::uvec2> indecies,
	    bool normal);

	Mesh(const char *path);

	Mesh(VertexBuffer vb,
	    IndexBuffer ib,
	    VertexArray vao,
	    unsigned int index_count);
	void draw(const ShaderProgram &shader_program) const;
	unsigned int get_index_count() const;
	void bind_vao() const;
	virtual const std::string class_name() const;
	ShaderProgram *shader_program = nullptr;

	std::vector<glm::vec3> get_vertecies() const;
	std::vector<glm::uvec3> get_indecies() const;
	bool normal;

  private:
	VertexBuffer vb;
	IndexBuffer ib;
	VertexArray vao;
	unsigned int index_count;
};
