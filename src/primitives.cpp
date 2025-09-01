#include "primitives.h"
#include "spdlog/spdlog.h"
#include <vector>
#include "utils.h"

const std::vector<glm::vec3> box_vertex_data = {
    // +X face (right)
    {0.5f, -0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},

    // -X face (left)
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},

    // +Y face (top)
    {-0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, -0.5f},

    // -Y face (bottom)
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, 0.5f},

    // +Z face (front)
    {-0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},

    // -Z face (back)
    {0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},
};

const std::vector<float> box_vertex_flat = {
    // +X face (right)
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,

    // -X face (left)
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,

    // +Y face (top)
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,

    // -Y face (bottom)
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,

    // +Z face (front)
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,

    // -Z face (back)
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f};

const std::vector<glm::uvec3> box_index_data = {
    // +X face (right)
    {0, 1, 2},
    {0, 2, 3},

    // -X face (left)
    {4, 5, 6},
    {4, 6, 7},

    // +Y face (top)
    {8, 9, 10},
    {8, 10, 11},

    // -Y face (bottom)
    {12, 13, 14},
    {12, 14, 15},

    // +Z face (front)
    {16, 17, 18},
    {16, 18, 19},

    // -Z face (back)
    {20, 21, 22},
    {20, 22, 23}};

const std::vector<unsigned int> box_index_flat = {
    // +X face (right)
    0,
    1,
    2,
    0,
    2,
    3,

    // -X face (left)
    4,
    5,
    6,
    4,
    6,
    7,

    // +Y face (top)
    8,
    9,
    10,
    8,
    10,
    11,

    // -Y face (bottom)
    12,
    13,
    14,
    12,
    14,
    15,

    // +Z face (front)
    16,
    17,
    18,
    16,
    18,
    19,

    // -Z face (back)
    20,
    21,
    22,
    20,
    22,
    23};

/*Box::Box() : Mesh(box_vertex_flat, box_index_flat, false) {
    spdlog::debug("Box created, with {0} vertecies, and {1} indecies!",
        box_vertex_data.size(),
        box_index_data.size());
} */

Box::Box() : Mesh(ObjectFile("cube.obj")) {}

const std::string Box::class_name() const {
	return std::string("Box");
}

std::vector<glm::vec3> DebugMesh::get_normal_vertecies(Mesh *mesh) {
	std::vector<glm::vec3> out;
	std::vector<glm::vec3> vertecies = mesh->get_vertecies();
	std::vector<glm::uvec3> indecies = mesh->get_indecies();

	for (glm::vec3 index : indecies) {
		glm::vec3 v0 = vertecies[index.x];
		glm::vec3 v1 = vertecies[index.y];
		glm::vec3 v2 = vertecies[index.z];

		glm::vec3 center = (v0 + v1 + v2) / 3.0f;

		glm::vec3 face_normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		out.push_back(center);
		out.push_back(center + face_normal * 2.0f);
		spdlog::debug("Center: ({0}, {1}, {2})", center.x, center.y, center.z);
	}
	spdlog::debug("count of debug vertecies: {}", out.size());
	return out;
}

std::vector<glm::uvec2> DebugMesh::get_normal_indecies(Mesh *mesh) {
	int count = mesh->get_index_count() / 3;
	std::vector<glm::uvec2> indecies;
	indecies.reserve(count);

	for (int i = 0; i < count * 2; i += 2) {
		indecies.push_back(glm::uvec2(i, i + 1));
	}
	spdlog::debug("count of debug indecies: {}", indecies.size());
	return indecies;
}

DebugMesh::DebugMesh(Mesh *mesh)
    : Mesh(get_normal_vertecies(mesh), get_normal_indecies(mesh), false) {}

DebugMesh::~DebugMesh() {}

const std::string DebugMesh::class_name() const {
	return std::string("Debug Mesh");
}
