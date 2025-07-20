#include "entity.h"
#include <glm/glm.hpp>

Entity::Entity(Mesh *mesh) : mesh(mesh) {}
Entity::Entity(Mesh *mesh, Transform transform)
    : mesh(mesh), transform(transform) {}
