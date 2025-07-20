#include <map>
#include <string>
#include "shader.h"
#include "mesh.h"

class ResourceManager {
  public:
	static std::map<std::string, ShaderProgram> shaders;
	static std::map<std::string, Mesh> meshes;

	static void load_shader_program(
	    std::string name, std::string vertex_path, std::string fragment_path);

	void load_mesh(std::string name, Mesh mesh);
	Mesh &get_mesh(std::string name);

	static ShaderProgram &get_shader_program(std::string name);
};
