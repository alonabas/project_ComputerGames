#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform int type;

out vec2 tex_coord;
out vec3 vertex;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(position,1.0f);
    vertex = vec3(modelMat * vec4(position,1.0f));
    vertex.z *= -2.0f;
    tex_coord = uv;
}
