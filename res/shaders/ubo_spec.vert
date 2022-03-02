#version 430 core
#define MAX_LIGHTS 16

/*
THIS SHADER IS ONLY MEANT AS A DECLARATION 
OF THE UNIVERSAL UBO STRUCTURES UTILISED.
*/

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;  // eye can be accessed through this
};

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
	float alignment;
};

layout(std140) uniform OK_Lights{
    OK_Light_Point light[MAX_LIGHTS];
};

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 uv;
in layout(location = 3) vec3 tangent;
in layout(location = 4) vec3 bitangent;

uniform layout(location = 0) mat4 MVP;
uniform layout(location = 1) mat4 TRS;
uniform layout(location = 2) mat3 NRM;
uniform layout(location = 3) vec3 eye;

out layout(location = 0) vec4 position_out;
out layout(location = 1) vec3 normal_out;
out layout(location = 2) vec2 uv_out;
out layout(location = 3) mat3 TBN;

void main()
{
    uv_out = uv;
    normal_out = normalize(NRM * normalize(normal));
    TBN = mat3(
        normalize(NRM * normalize(tangent)),
        normalize(NRM * normalize(bitangent)),
        normal_out
    );
    gl_Position = MVP * vec4(position, 1.0);
    position_out = TRS * vec4(position, 1.0);
}
