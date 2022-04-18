#version 430 core

in layout(location = 0) vec4 position;
in layout(location = 1) vec2 uv;

uniform layout(binding = 0) sampler2D _color;
uniform layout(binding = 1) sampler2D _depth;

out vec4 color;

void main()
{
    color = vec4(texture(_color, uv).rgb, 1);
}
