#include "entity.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

Entity::Entity(const char *name, Mesh *mesh) : name(name), mesh(mesh) {}
Entity::Entity(const char *name, Mesh *mesh, Transform transform)
    : name(name), mesh(mesh), transform(transform) {}
