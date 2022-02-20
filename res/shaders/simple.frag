#version 430 core

#define MAX_LIGHTS 8
#define EPSILON 0.00000000001
#define SHADOW_SMOOTHING_FACTOR 50

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

uniform layout(location = 8) vec3 ambient_intensity = vec3(0.1, 0.1, 0.1);
uniform layout(location = 9) vec3 specular_intensity = vec3(0.95, 0.95, 0.95);
uniform layout(location = 10) PointLight light[MAX_LIGHTS];

out vec4 color;

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }

float shadow(in vec3 to_light) {
    vec3 to_ball = ball_info.xyz - world_position;

    vec3 light_dir = normalize(to_light);
    float scalar_projection = dot(to_ball, light_dir);
    vec3 rejection = to_ball - scalar_projection * light_dir;
    
    // the rate at which this light ray goes through the ball.
    float shadow_smoothing = min(ball_info.w * 2, (length(to_ball) + ball_info.w) / 200);
    float ray_through_rate = smoothstep(-shadow_smoothing, shadow_smoothing, ball_info.w - length(rejection));
    
    // Will become 1 when should cast shadow
    float should_cast = step(0, scalar_projection)               // Opposite directions
                      * step(length(to_ball) - ball_info.w, length(to_light)); // Light closer than ball

    return 1.0 - (ray_through_rate * should_cast);
}

// Calculate light contribution (world space)
vec3 pointlight(in PointLight light) {
    
    // Discard contribution of lights outside of box
    vec3 extents = box_info[1].xyz;
    vec3 k = step(0, extents - abs(box_info[0] - light.position));
    float inside = k.x * k.y * k.z;
    
    // Calculate shadows
    vec3 to_light = light.position - world_position;
    float in_light_rate = shadow(to_light);

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

    return (diffuse + specular ) * attenuation * in_light_rate * inside;
}

void main()
{
    vec3 lights = ambient_intensity;
    color = vec4(0.5 * normalize(normal) + 0.5, 1.0);
//    color.rgb = vec3(0.25);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        lights += pointlight(light[i]);
    }
    color = vec4(lights * color.rgb + dither(uv), color.a);
}