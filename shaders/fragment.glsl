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
    vec3 N = normalize(Normal);
    vec3 V = normalize(view_pos - FragPos);

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for (int i = 0; i < light_size; i++)
    {
        vec3 L = normalize(lights[i].position - FragPos);

        // Diffuse term
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * mat_diffuse * lights[i].color * lights[i].intensity;

        // Specular term (Phong)
        vec3 R = reflect(-L, N);
        float spec = 0.0;
        if(diff > 0.0)
        {
            spec = pow(max(dot(R, V), 0.0), mat_shininess);
        }
        vec3 specular = spec * mat_specular * lights[i].color * lights[i].intensity;

        totalDiffuse += diffuse;
        totalSpecular += specular;
    }

    vec3 color = totalDiffuse + totalSpecular;

    FragColor = vec4(color, 1.0);

} 
