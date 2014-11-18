#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec2 tex_coord;
out vec3 vert;
void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(position,1.0f);
    tex_coord = uv;
    vert = vec3(modelMat * vec4(position,1.0f));
}
