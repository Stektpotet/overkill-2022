#version 430 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 uv;

uniform layout(location = 0) mat4 MVP;
uniform layout(location = 1) mat4 TRS;
uniform layout(location = 2) mat3 NRM;
uniform layout(location = 3) vec3 eye;

out layout(location = 0) vec4 position_out;
out layout(location = 1) vec3 normal_out;
out layout(location = 2) vec2 uv_out;

void main()
{
    uv_out = uv;
    normal_out = normalize(NRM * normalize(normal));
    gl_Position = MVP * vec4(position, 1.0);
    position_out = TRS * vec4(position, 1.0);
}
