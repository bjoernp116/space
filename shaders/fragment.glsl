#version 330 core
out vec4 FragColor;

void main()
{
    //float ambientStrength = 0.1;
    //vec3 ambient = ambientStrength * lightColor;

    vec3 result = vec3(0.5, 0.5f, 0.5f);//ambient * objectColor;

    FragColor = vec4(result, 1.0f);
} 
