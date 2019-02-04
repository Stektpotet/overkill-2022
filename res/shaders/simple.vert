#version 430 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal_in;

uniform layout(location = 3) mat4 MVP;

out layout(location = 0) vec3 normal_out;

void main()
{
    normal_out = normal_in;
    gl_Position = MVP * vec4(position, 1.0f);
}
