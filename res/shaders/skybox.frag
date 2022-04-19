#version 430 core
#define MAX_LIGHTS 16
#define PI 3.1415926535897932384626433832795

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
uniform layout(location = 9) vec3 specular_intensity = vec3(1., 1., 1.);


uniform layout(binding = 8) sampler2D dayTimeSunColorRamp;

out vec4 color;

void main() {
    //approximate for getting sun colour sampling unit out of direction
    float t = max(0.15 + dot(-sun.direction.xyz, vec3(0,1,0)), 0);
    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(t, 0.5)).rgb;

    vec3 fragDirection = -world_position;

    float size = 4;
    float sunPower = pow(max(dot(normalize(fragDirection), normalize(sun.direction.xyz)), 0.0), 512) * size;
    vec3 sunlight = dayTimeSunColor * sunPower;
    
    vec3 nice_blue_sky = mix(dayTimeSunColor * max(t, 0.1), vec3(111.0/255, 183.0/255, 231.0/255) * max(t, 0.1), t);

    vec3 tex = nice_blue_sky;//texture(skyTexture, uv).rgb;
    color = vec4(tex+sunlight, 1);
}
