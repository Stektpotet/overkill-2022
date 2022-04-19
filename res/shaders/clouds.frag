#version 430 core

in layout(location = 0) vec2 uv;
in layout(location = 1) vec3 view_direction;

layout(std140) uniform OK_Clouds{
    vec4 bounds_min;
    vec4 bounds_max;
} clouds;

layout(std140) uniform OK_Commons{
    mat4 projection;
    mat4 view;
    mat4 view_projection;
    mat4 view_projection_inv;
    vec4 cam_direction;
    vec4 cam_settings;
    vec4 time;
};
uniform layout(binding = 0) sampler2D _color;
uniform layout(binding = 1) sampler2D _depth;
uniform layout(binding = 3) sampler3D worley;
uniform layout(location = 5) float density_threshold = 0.5;
uniform layout(location = 6) float density_multiplier = 10;
uniform layout(location = 7) int num_steps = 200;

#define eye vec3(view[3])

out vec4 color;

float linearize_depth(in float z) 
{
    float near = cam_settings.x;
    float far = cam_settings.y;

    return 1/z - 1/near / (1/far - 1/near);
}


vec2 ray_box_dst() 
{
    vec3 t0 = (clouds.bounds_min.xyz - eye) / normalize(view_direction.xyz);
    vec3 t1 = (clouds.bounds_max.xyz - eye) / normalize(view_direction.xyz);
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float dstA = max(max(tmin.x, tmin.y), tmin.z);
    float dstB = min(min(tmax.x, tmax.y), tmax.z);

    float dst_to_box = max(0, dstA);
    float dst_inside_box = max(0, dstB - dst_to_box);
    return vec2(dst_to_box, dst_inside_box);
}

float sample_density(vec3 pos)
{
    vec3 uvw = pos * vec3(73) * 0.001 + vec3(213) * 0.01;
    vec4 noise_shape = texture(worley, uvw);
    float density = max(0, noise_shape.r - density_threshold) * density_multiplier;
    return density;
}

void main() {
    
    vec3 ray_dir = normalize(view_direction.xyz);
    vec2 ray_box_info = ray_box_dst();

    color = vec4(texture(_color, uv).rgb, 1);
    float depth = linearize_depth(texture(_depth, uv).r) * length(view_direction.xyz);
    
    float dst_to_box = ray_box_info.x;
    float dst_inside_box = ray_box_info.y;


    float dst_traveled = 0;
    float step_size = dst_inside_box / num_steps;
    float dst_limit = min(depth - dst_to_box, dst_inside_box);

    float tot_density = 0;
    while (dst_traveled < dst_limit) 
    {
        vec3 ray_position = eye + ray_dir * (dst_to_box + dst_inside_box);
        tot_density += sample_density(ray_position) * step_size;
        dst_traveled += step_size;
    }
    float transmittance = exp(-tot_density);
    color *= transmittance;
}
