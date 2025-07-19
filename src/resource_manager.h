#include <map>
#include <string>
#include "buffer.h"
#include "shader.h"

class ResourceManager {
  public:
	static std::map<std::string, ShaderProgram> shaders;
	static std::map<std::string, IndexBuffer> index_buffers;
	static std::map<std::string, VertexBuffer> vertex_buffers;
	static VertexArray vertex_array;

	static void set_vertex_array(VertexArray vao);

	static void load_shader_program(
	    std::string name, std::string vertex_path, std::string fragment_path);

	static void load_index_buffer(std::string name, IndexBuffer &&index_buffer);
	static void load_vertex_buffer(std::string name,
	    VertexBuffer &&index_buffer);

	static ShaderProgram &get_shader_program(std::string name);
	static IndexBuffer &get_index_buffer(std::string name);
	static VertexBuffer &get_vertex_buffer(std::string name);
	static VertexArray &get_vertex_array();
};
