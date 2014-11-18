#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

out vec2 tex_coord;
out vec3 vert;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    gl_Position = vec4(position,1.0f);
    tex_coord = uv;
    vert = position;
}
