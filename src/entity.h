#pragma once

#include "mesh.h"
#include "transform.h"
#include "material.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Entity {
  public:
	const char *name;
	Mesh *mesh;
	Transform transform;
	Material material;

	Entity(const char *name, Mesh *mesh);
	Entity(const char *name, Mesh *mesh, Transform transform);
};
