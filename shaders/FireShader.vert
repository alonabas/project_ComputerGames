#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 WVP;
uniform vec3 cameraPosition;

out vec3 col;

void main()
{
    gl_Position = vec4(position,1.0);
    col = color;
}
