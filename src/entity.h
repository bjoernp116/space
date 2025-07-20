#pragma once
#include "mesh.h"
#include "transform.h"
#include <glm/glm.hpp>

class Entity {
  public:
	Mesh *mesh;
	Transform transform;

	Entity(Mesh *mesh);
	Entity(Mesh *mesh, Transform transform);
};
