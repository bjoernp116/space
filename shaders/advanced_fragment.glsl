#version 450 core



in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int light_size;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform float mat_shininess;

uniform vec3 view_pos;
uniform Light lights[40];

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lights[0].color;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(lights[0].position - FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * lights[0].color;

    vec3 result = (diffuse + ambient) * mat_diffuse;
    FragColor = vec4(result, 1.0f);
} 
