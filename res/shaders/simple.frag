#version 430 core

#define MAX_LIGHTS 8
#define EPSILON 0.00000000001

in layout(location = 0) vec3 world_position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 uv;


struct PointLight {
    vec3 position;
    vec3 intensities;
	float constant;
	float linear;
	float quadratic;
};

uniform layout(location = 0) mat4 MVP;
uniform layout(location = 1) mat4 TRS;
uniform layout(location = 2) mat3 NRM;
uniform layout(location = 3) vec3 eye;

uniform layout(location = 5) vec4 ball_info;  // ball_info.xyz = position,  ball_info.w = radius
uniform layout(location = 6) mat2x3 pad_info; // pad_info[0] = position,    pad_info[1] = dimensions
uniform layout(location = 7) mat2x3 box_info; // box_info[0] = position,    pad_info[1] = dimensions

uniform layout(location = 8) vec3 ambient_intensity = vec3(0.01, 0.01, 0.01);
uniform layout(location = 9) vec3 specular_intensity = vec3(1.0, 0.95, 0.95);
uniform layout(location = 10) PointLight light[MAX_LIGHTS];

out vec4 color;

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }


float shadow(in vec3 light_pos) {
    vec3 to_light = light_pos - world_position;
    vec3 to_ball = ball_info.xyz - world_position;
    vec3 to_pad = pad_info[0] - world_position;

    vec3 light_dir = normalize(to_light);
    float scalar_projection = dot(to_ball, light_dir);
    vec3 rejection = to_ball - scalar_projection * light_dir;

    float rejection_rate = smoothstep(0.0, 0.01, ball_info.w - length(rejection));
    
    // Will become 0 when light is closer to the fragment than the ball
    float light_closer = step(0, length(to_light) - (length(to_ball)));

    return 1.0 - (rejection_rate *  step(0, scalar_projection));
}

// Calculate light contribution (world space)
vec3 pointlight(in PointLight light) {

    vec3 to_light = light.position - world_position;
    float occludsion_rate = shadow(to_light);
    
    vec3 extents = box_info[1].xyz;
    vec3 k = step(0, extents - abs(box_info[0] - light.position));
    float inside = 1;

    // Diffussion
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(to_light);
    float diffusion = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diffusion * light.intensities;

    // Specularity
    vec3 view_dir = normalize(eye - world_position);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float specular_power = pow(max(dot(view_dir, reflect_dir), 0.0), 12);
    vec3 specular = specular_intensity * specular_power * light.intensities;

    // Attenuation
    float distance = length(light.position - world_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance + EPSILON);

    return (diffuse + specular) * attenuation * occludsion_rate * inside;
}





void main()
{
    vec3 lights = ambient_intensity;
    color = vec4(0.25 * normalize(normal) + 0.5, 1.0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        lights += pointlight(light[i]);
    }
    lights += dither(uv);
    color = vec4(lights * color.rgb, color.a);
}