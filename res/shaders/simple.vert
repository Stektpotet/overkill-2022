#version 430 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal_in;
in layout(location = 2) vec2 textureCoordinates_in;

uniform layout(location = 3) mat4 MVP;

out layout(location = 0) vec3 normal_out;
out layout(location = 1) vec2 textureCoordinates_out;

void main()
{
    normal_out = normal_in;
    textureCoordinates_out = textureCoordinates_in;
    gl_Position = MVP * vec4(position, 1.0f);
}
