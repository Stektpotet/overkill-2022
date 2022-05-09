#version 430 core
#define MAX_LIGHTS 16
#define EPSILON 0.00000000001

in layout(location = 0) vec2 uv;
in layout(location = 1) vec3 view_direction;

layout(std140) uniform OK_Clouds{
    vec4 bounds_min;
    vec4 bounds_max;
} clouds;

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

uniform layout(binding = 0) sampler2D _color;
uniform layout(binding = 1) sampler2D _depth;
uniform layout(binding = 3) sampler3D worley;
uniform layout(binding = 8) sampler2D dayTimeSunColorRamp;
uniform layout(location = 5) float density_offset = -0.67;
uniform layout(location = 6) float density_multiplier = 0.46;
//uniform layout(location = 7) float density_offset = -0.5;
uniform layout(location = 8) int num_light_steps = 8;

const float absorption = 0.8;
const float darkness_threshold = 0.1;
const float forward_scattering = 0.7;
const float back_scattering = 0.3;
const float base_brightness = 1;
const float phase_factor = 0.75;
const float ray_offset_strength = 21;

const vec3 ambient_intensity = vec3(0.1, 0.1, 0.1);
    


out vec4 color;


float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }

float linearize_depth(in float depth) 
{
    float zNear = zbuffer_params.x;
    float zFar  = zbuffer_params.y;
    
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

float hg(float a, float g) {
    float g2 = g*g;
    return (1-g2) / (4*3.1415*pow(1+g2-2*g*(a), 1.5));
}

float phase(float a) {
    float blend = .5;
    float hgBlend = hg(a, forward_scattering) * (1-blend) + hg(a,-back_scattering) * blend;
    return base_brightness + hgBlend*phase_factor;
}

float beer(float d) {
    float beer = exp(-d);
    return beer;
}

float remap01(float v, float low, float high) {
    return (v-low)/(high-low);
}

vec2 ray_box_dist(vec3 p, vec3 direction) 
{
    vec3 t0 = (clouds.bounds_min.xyz - p) / normalize(direction.xyz);
    vec3 t1 = (clouds.bounds_max.xyz - p) / normalize(direction.xyz);
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float distA = max(max(tmin.x, tmin.y), tmin.z);
    float distB = min(min(tmax.x, tmax.y), tmax.z);

    float dist_to_box = max(0, distA);
    float dist_inside_box = max(0, distB - dist_to_box);
    return vec2(dist_to_box, dist_inside_box);
}

const mat3 rot45 = mat3(
        0.70710678119, -0.70710678119,0,
        0.70710678119, 0.70710678119, 0,
        0, 0, 1) * 
        mat3(0.70710678119,0, 0.70710678119,
        0, 1, 0,
        -0.70710678119, 0, 0.70710678119) * 
        mat3(1, 0, 0,
        0, 0.70710678119, -0.70710678119,
        0, 0.70710678119, 0.70710678119
        );

#define OCTAVES 3

float fbmd(in vec3 uvw)
{
    float value = 0.0;
    float normalization_factor = 0.0;
    float scale = 0.5;

    for (int i = 0; i < OCTAVES; i++)
    {
        value += texture(worley, uvw).r * scale;
        normalization_factor += scale;
        uvw = rot45 * uvw * 2.0;
        scale *= 0.5;
    }
    return value / normalization_factor;
}

float sample_density(vec3 pos)
{
    vec3 uvw = pos * vec3(1, 1, 1) * 0.0006;
          
    vec3 str = rot45 * uvw * 0.1 + vec3(0.012, 0.0352, 0.054) * time.w * .4333;

    uvw += vec3(0.0153, 0.0162, 0.08302) * time.w;

    vec4 noise_shapes = texture(worley, uvw.xzy);
    float main_shape = noise_shapes.r;
    vec3 noise = noise_shapes.gba;


    vec3 strength = smoothstep(clouds.bounds_min.xyz, clouds.bounds_min.xyz + 10, pos) 
                     - smoothstep(clouds.bounds_max.xyz - 10,clouds.bounds_max.xyz, pos);
    float s = strength.r * strength.g * strength.b;


//    vec4 noise_shape2 = texture(worley, str.xzy);
    float density = max(0, main_shape + density_offset) * density_multiplier * s;
    if (density > 0) {
        vec4 noise_shapes2 = texture(worley, str.xzy);

        float density2 = max(0, noise_shapes2.r + density_offset) * density_multiplier;
        float density3 = max(0, noise.r * noise.g + density_offset) * density_multiplier;
        density = max(0, density - density2 - density3);
    }
    return density;
}


float light_march(in vec3 pos)
{
    vec3 dir_to_light = -sun.direction.xyz;
    float dist_inside_box = ray_box_dist(pos, dir_to_light).y;

//    float step_size = dist_inside_box/num_light_steps;
    const float step_size = 11;
    float tot_density = 0;
    for (int s = 0; s < num_light_steps; s++)
    {
        pos += dir_to_light * step_size;
        tot_density += max(0, sample_density(pos) * step_size);
    }
    float transmittance = exp(-tot_density * absorption);
    return darkness_threshold + transmittance * (1-darkness_threshold);
}

void main() {

    vec3 ray_dir = normalize(view_direction.xyz);
    vec2 ray_box_info = ray_box_dist(eye, ray_dir);
    float dist_to_box = ray_box_info.x;
    float dist_inside_box = ray_box_info.y;

    float depth = linearize_depth(texture(_depth, uv).r) + dither(uv);
    
    vec3 ray_position = eye;
    vec3 entry_point = ray_position + ray_dir * dist_to_box;

    float random_offset = rand(uv) * 10;
    float phase_val = phase(dot(ray_dir, -sun.direction.xyz));
    
//    color = vec4(texture(_color, uv).rgb, 1);
//    if (dist_inside_box > 0) 
//    {
//        color.rgb = vec3(0);
//    }
//
//    return;

    const float step_size = 11;
    float dist_limit = min(depth - dist_to_box, dist_inside_box);
    
    vec3 light_strength = vec3(0);
    float transmittance = 1;

    
    float k = (0.25 - sun.direction.y) / 1.25;
    float t = smoothstep(0, 1, (2-k)*k);
    float sunlight_contribution = smoothstep(0.001, 0.13, t);

    float dist_travelled = random_offset;
    while (dist_travelled < dist_limit) 
    {
        ray_position = entry_point + ray_dir * dist_travelled;
        float density = sample_density(ray_position);

        if (density > 0) 
        {
            vec3 light_transmittance = mix(ambient_intensity, vec3(light_march(ray_position)), sunlight_contribution);
            light_strength += density * step_size * transmittance * light_transmittance * phase_val;
            transmittance *= exp(-density * step_size * absorption);

            if (transmittance < 0.01) 
            {
                break;
            }
        }
        dist_travelled += step_size + rand(ray_position.xy);
    }

    vec3 dayTimeSunColor = texture(dayTimeSunColorRamp, vec2(t, 0.5)).rgb;

    color = vec4(texture(_color, uv).rgb, 1);

    vec3 cloud_light = mix(light_strength, light_strength * (dayTimeSunColor * sunlight_contribution), sunlight_contribution);
    color = vec4(color.rgb * transmittance + cloud_light, color.a);
}
