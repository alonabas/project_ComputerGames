#version 330

#define MY_PI 3.1415926
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 normalMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform int type;

out vec3 ambient_light;
out vec3 normal_vertex;
out vec3 vertex;
out vec2 tex_coord;

void main()
{
    if(type == 0){
        ambient_light = vec3(0.1, 0.1, 0.1);
    }
    else{
        ambient_light = vec3(0.25,0.25,0.25);
    }
    
    vertex = vec3(viewMat * modelMat * vec4(position,1.0f));
    normal_vertex = (normalMat * vec4(normal,1.0f)).xyz;
    tex_coord = uv;
    
    gl_Position = projMat * viewMat * modelMat * vec4(position,1.0f);
}
