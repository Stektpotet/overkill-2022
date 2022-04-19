#version 430 core
in layout(location = 0) vec2 position;
out layout(location = 0) vec2 uv;
out layout(location = 1) vec3 view_direction;

layout(std140) uniform OK_Commons{
    mat4 projection;
    mat4 view;
    mat4 view_projection;
    mat4 view_projection_inv;
    vec4 cam_direction;
    vec4 cam_settings;
    vec4 time;
};
#define eye vec3(view[3])

void main()
{
    uv = (position + 1) * 0.5;
    vec4 ray_nds = vec4(uv * 2.0 - 1.0, 0.0, -1.0);
    vec3 a = normalize((inverse(projection) * ray_nds).xyz);
    view_direction = (inverse(view) * vec4(a, 0)).xyz;
    gl_Position = vec4(position, 0.0 , 1.0);
}
