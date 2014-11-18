#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 normalMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 eye_position;

const float Air = 1.0;
const float Glass = 1.03;
const float Eta = Air / Glass;
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));

out vec3 normal_dir;
out vec3 eye_dir;
out vec3 vertex;
out vec3 v_reflection;
out vec3 v_refraction;
out float v_fresnel;

void main()
{
    vec4 temp = modelMat * vec4(position,1.0f);
    vertex = vec3(temp);
    
    normal_dir = normalize(vec3(normalMat * vec4(normal, 0.0)));
    eye_dir = normalize(vertex - eye_position);
    
    v_refraction = refract(eye_dir, normal_dir, Eta);
    v_reflection = reflect(eye_dir, normal_dir);
    v_fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-eye_dir, normal_dir)), 5.0);
    
    gl_Position = projMat * viewMat * modelMat * vec4(position,1.0f);
}