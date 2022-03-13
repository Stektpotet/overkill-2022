#version 430 core

in layout(location = 0) vec3 view_position;
in layout(location = 1) vec3 normal;
noperspective in layout(location = 2) vec2 uv;

uniform layout(location = 0) mat4 MVP;
uniform layout(location = 1) mat4 TRS;
uniform layout(location = 2) mat3 NRM;
uniform layout(binding = 0) sampler2D charmap;

out vec4 color;

void main()
{
    color = texture(charmap, uv);
    color *= vec4(
        0.5 * sin((view_position.x + view_position.y)*0.1)*cos(view_position.y*0.033) + 0.5, 
        0.5 * sin(view_position.x * 0.05) + 0.5, 
        0.5 * cos(view_position.y * 0.05 * view_position.x) + 0.5, 
        1
    );
//    color = vec4(uv, 0, 1);
}