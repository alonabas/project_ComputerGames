#version 330

#define MY_PI 3.1415926
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 normalMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


out vec3 normal_vertex;
out vec3 vertex;
out vec2 tex_coord;

void main()
{
    // current position, whithout projection
    vertex = vec3(modelMat * position);
    // normal after viewAt and model
    normal_vertex = (normalMat * vec4(normal,1.0f)).xyz;
    tex_coord = uv;
    
    gl_Position = modelMat * position;
}
