#version 430 core
in layout(location = 0) vec2 uv;
uniform layout(binding = 0) sampler2D _color;
uniform layout(binding = 1) sampler2D _depth;
out vec4 color;
void main(){
color = vec4(texture(_color, uv).rgb, 1);
//color *= vec4(color.rgb * smoothstep(1.2, 0.9, length(uv * 2 - 1)), 1);
}
