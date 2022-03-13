#version 430 core

#define MAX_LIGHTS 16
#define EPSILON 0.00000000001
#define SHADOW_CASTER_MAX_DISTANCE 250

in layout(location = 0) vec3 world_position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 uv;
in layout(location = 3) mat3 TBN;

struct OK_Light_Point {
    vec4 position;
    vec4 intensities;
	float constant;
	float linear;
	float quadratic;
	float _UNUSED;
};

layout(std140) uniform OK_Lights{
    OK_Light_Point light[MAX_LIGHTS];
};


uniform layout(location = 0) mat4 MVP;
uniform layout(location = 1) mat4 TRS;
uniform layout(location = 2) mat3 NRM;
uniform layout(location = 3) vec3 eye;

uniform layout(location = 5) vec4 ball_info;  // ball_info.xyz = position,  ball_info.w = radius
uniform layout(location = 6) mat2x3 pad_info; // pad_info[0] = position,    pad_info[1] = dimensions
uniform layout(location = 7) mat2x3 box_info; // box_info[0] = position,    pad_info[1] = dimensions

uniform layout(location = 8) vec3 ambient_intensity = vec3(0.1, 0.1, 0.1);
uniform layout(location = 9) vec3 specular_intensity = vec3(1., 1., 1.);
uniform float normalmap_strength = 10.;

uniform layout(binding = 1) sampler2D albedomap;
uniform layout(binding = 2) sampler2D normalmap;
uniform layout(binding = 3) sampler2D specularmap;

out vec4 color;

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }

float shadow(in vec3 to_light) {
    vec3 to_ball = ball_info.xyz - world_position;

    vec3 light_dir = normalize(to_light);
    float scalar_projection = dot(to_ball, light_dir);
    vec3 rejection = to_ball - scalar_projection * light_dir;
    
    // the rate at which this light ray goes through the ball.
    float shadow_smoothing = min(ball_info.w * 2, (length(to_ball) + ball_info.w) / SHADOW_CASTER_MAX_DISTANCE);
    float ray_through_rate = smoothstep(-shadow_smoothing, shadow_smoothing, ball_info.w - length(rejection));
    
    // Will become 1 when should cast shadow
    float should_cast = step(0, scalar_projection)               // Opposite directions
                      * step(length(to_ball) - ball_info.w, length(to_light)); // Light closer than ball

    return 1.0 - (ray_through_rate * should_cast);
}

// Calculate light contribution (world space)
vec3 pointlight(in OK_Light_Point light) {
    
    // Discard contribution of lights outside of box
    vec3 extents = box_info[1].xyz;
    vec3 k = step(0, extents - abs(box_info[0] - light.position.xyz));
    float inside = k.x * k.y * k.z;
    
    // Calculate shadows
    vec3 to_light = light.position.xyz - world_position;
    float in_light_rate = shadow(to_light);

    // Diffussion
    vec3 norm = (texture(normalmap, uv).rgb * 2.0 - 1.0);
    // Scale normal deviation by the normalmap strength
    norm = normalize(TBN * normalize(vec3(norm.rg * normalmap_strength, norm.b)));
    vec3 light_dir = normalize(to_light);
    float diffusion = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diffusion * light.intensities.rgb;

    // Specularity
    float roughness = texture(specularmap, uv).r;

    vec3 view_dir = normalize(eye - world_position);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float specular_power = pow(max(dot(view_dir, reflect_dir), 0.0), 5.0 / roughness * roughness);
    vec3 specular = specular_intensity * specular_power * light.intensities.rgb;

    // Attenuation
    float distance = length(light.position.xyz - world_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance + EPSILON);

    return (diffuse + specular) * attenuation * in_light_rate * inside;
}

void main()
{
    vec3 albedo = texture(albedomap, uv).rgb;
    vec3 lights = ambient_intensity;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        lights += pointlight(light[i]);
    }
    color = vec4(lights * albedo.rgb + dither(uv), 1);
}