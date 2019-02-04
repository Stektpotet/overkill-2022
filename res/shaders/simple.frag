#version 430 core

in layout(location = 0) vec3 normal;

out vec4 color;

void main()
{
    float intensity = dot(normalize(normal), vec3(0, 0, 1));
    color = vec4(0.5 * normal + 0.5, 1.0);
    //color = vec4(intensity, intensity, intensity, 1.0f);
}
