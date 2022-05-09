#version 430 core
in layout(location = 0) vec2 position;
out layout(location = 0) vec2 uv;
out layout(location = 1) vec3 view_direction;

layout(std140) uniform OK_Commons{
    mat4 projection;
    mat4 view;
    mat4 view_projection;
    mat4 view_inv;
    mat4 projection_inv;
    mat4 view_projection_inv;
    vec4 projection_params;
    vec4 zbuffer_params;
    vec4 camera_position;
    vec4 camera_direction;
    vec4 time;
};
#define eye vec3(camera_position)
void main()
{
    uv = (position + 1) * 0.5;
    vec2 clip_slice = uv * 2.0 - 1.0;
    vec4 ray_near = vec4(clip_slice, -1.0, 1.0);
    vec4 ray_far = vec4(clip_slice, 1.0, 1.0);

    vec4 near_res = view_projection_inv * ray_near;
    vec4 far_res = view_projection_inv * ray_far;

    near_res /= near_res.w;
    far_res /= far_res.w;
    view_direction = vec3(far_res - near_res);

    gl_Position = vec4(position, 0.0 , 1.0);
}
