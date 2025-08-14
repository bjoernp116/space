#pragma once

#include "mesh.h"

class Box : public Mesh {
  public:
	Box();
	const std::string class_name() const override;
};

class DebugMesh : public Mesh {
  public:
	DebugMesh(Mesh *mesh);
	~DebugMesh();

	const std::string class_name() const override;

  private:
	static std::vector<glm::uvec2> get_normal_indecies(Mesh *mesh);
	static std::vector<glm::vec3> get_normal_vertecies(Mesh *mesh);
};
