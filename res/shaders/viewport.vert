#version 430 core

in layout(location = 0) vec2 position;

out layout(location = 0) vec4 position_out;
out layout(location = 1) vec2 uv;

void main()
{
    position_out = vec4(position, 0.0 , 1.0);
    uv = position;
    gl_Position = position_out;
}
