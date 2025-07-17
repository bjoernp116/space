#include "shader.h"
#include "utils.h"

Shader::Shader(const char* path, GLenum shader_type) {
    src = read_file(path);
    type = shader_type;
    id = glCreateShader(shader_type);
}

Shader::~Shader() {
    glDeleteShader(id);
}

void Shader::compile_shader() {
    const char* src_point = src.c_str();
    glShaderSource(id, 1, &src_point, nullptr);
    glCompileShader(id);
}

bool Shader::status() {
    int  success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "(" << src << ")"<< std::endl;
        
        return false;
    }
    return true;
}

unsigned int link_shaders(Shader s1, Shader s2) {
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, s1.id);
    glAttachShader(shader_program, s2.id);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    return shader_program;
}



