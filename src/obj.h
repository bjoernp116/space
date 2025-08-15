

#include "glm/glm.hpp"
#include <string>
#include <vector>
class ObjectFile {

	std::vector<glm::vec3> vertecies;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indecies;
	unsigned int face_size;
	std::string path;

  public:
	ObjectFile(const char *);

	template <typename T> std::vector<T> get_vertecies() const;
	template <typename T> std::vector<T> get_normals() const;
	template <typename T> std::vector<T> get_indecies() const;
	unsigned int get_face_size() const;
};
