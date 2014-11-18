#version 330

#define MY_PI 3.1415926
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

const vec3 ambientColor = vec3(1.0, 1.0, 1.0); // Ambient light color

uniform vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient coefficient
uniform mat4 normalMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


out vec3 ambient_light;
out vec3 normal_vertex;
out vec3 vertex;
out vec2 tex_coord;
out vec3 pos;

void main()
{
    ambient_light = ka * ambientColor;
    
    vertex = vec3(viewMat * modelMat * position);
    normal_vertex = (normalMat * vec4(normal,1.0f)).xyz;
    tex_coord = uv;
    pos = (modelMat * position).xyz;
    
    gl_Position = projMat * viewMat * modelMat * position;
}
