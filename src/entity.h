#pragma once

#include "mesh.h"
#include "transform.h"
#include "material.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Entity {
  public:
	Mesh *mesh;
	Transform transform;
	Material material;

	Entity(Mesh *mesh);
	Entity(Mesh *mesh, Transform transform);
};
