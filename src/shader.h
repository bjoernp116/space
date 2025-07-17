#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>


class Shader {
public:
    unsigned int id;
    std::string src;
    int type;

    Shader(const char* path, GLenum shader_type);
    ~Shader();
    void compile_shader();
    bool status();

    void SetFloat (const char *name, float value, bool useShader = false);
    void SetInteger (const char *name, int value, bool useShader = false);
    void SetVector2f (const char *name, float x, float y, bool useShader = false);
    void SetVector2f (const char *name, const glm::vec2 &value, bool useShader = false);
    void SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
    void SetVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
    void SetVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
    void SetMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);
};

unsigned int link_shaders(Shader s1, Shader s2);
