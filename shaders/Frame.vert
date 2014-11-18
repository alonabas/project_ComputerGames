#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 modelMat;

out vec2 tex_coord;

void main()
{
    gl_Position = modelMat * vec4(position,1.0f);
    tex_coord = uv;
}
