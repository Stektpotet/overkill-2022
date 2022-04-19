#version 430 core

#define MAX_LIGHTS 16
#define EPSILON 0.00000000001
#define SHADOW_CASTER_MAX_DISTANCE 250

in layout(location = 0) vec3 world_position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 uv;

struct OK_Light_Directional {
	vec4 direction;
	vec4 intensities;
};

struct OK_Light_Point {
    vec4 position;
    vec4 intensities;
	float constant;
	float linear;
	float quadratic;
	float _UNUSED;
};

layout(std140) uniform OK_Lights{
    OK_Light_Directional sun;
    OK_Light_Point light[MAX_LIGHTS];
};

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

uniform layout(location = 0) mat4 MVP;
uniform layout(location = 1) mat4 TRS;
uniform layout(location = 2) mat3 NRM;

uniform layout(location = 8) vec3 ambient_intensity = vec3(0.1, 0.1, 0.1);
uniform layout(location = 9) vec3 specular_intensity = vec3(0.3, 0.3, 0.3);


uniform layout(binding = 8) sampler2D dayTimeSunColorRamp;
uniform layout(binding = 9) sampler2D terrain_color_ramp;


out vec4 color;

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }


vec3 directional_light(in OK_Light_Directional light) {
	//Diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = -normalize(light.direction.xyz);
    float diffusion = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diffusion * light.intensities.rgb;
    
	//Specularity
    vec3 view_dir = normalize(eye - world_position);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float specular_power = pow(max(dot(view_dir, reflect_dir), 0.0), 20);
    vec3 specular = specular_intensity * specular_power * light.intensities.rgb;

	return (diffuse + specular) * max(0.25 + dot(light_dir, vec3(0,1,0)), 0);
}

// Calculate light contribution (world space)
vec3 pointlight(in OK_Light_Point light) {
    
    // Calculate shadows
    vec3 to_light = light.position.xyz - world_position;

    // Diffussion
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(to_light);
    float diffusion = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diffusion * light.intensities.rgb;

    // Specularity
    vec3 view_dir = normalize(eye - world_position);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float specular_power = pow(max(dot(view_dir, reflect_dir), 0.0), 20);
    vec3 specular = specular_intensity * specular_power * light.intensities.rgb;

    // Attenuation
    float distance = length(light.position.xyz - world_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance + EPSILON);

    return (diffuse + specular) * attenuation;
}

vec3 terrain_color()
{
    return texture(terrain_color_ramp, vec2(world_position.y/20, 0)).rgb;
}

void main()
{
    float t = max(0.25 + dot(-sun.direction.xyz, vec3(0,1,0)), 0);
    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(t, 0.5)).rgb;


    vec3 lights = ambient_intensity;
    lights += directional_light(sun);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        lights += pointlight(light[i]);
    }
    color = vec4(terrain_color() * lights + dither(uv), 1);
}
