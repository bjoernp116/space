#include <map>
#include <string>
#include "shader.h"


class ResourceManager {
    public:
        static std::map<std::string, Shader> shaders;
        
        static Shader load_shader(
            std::string name,
            const char* path,
            GLenum shader_type
        );
        static Shader get_shader(std::string name);
};
