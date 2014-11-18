#version 330

layout (points) in;

layout (triangle_strip) out;
layout (max_vertices = 44) out;

uniform mat4 modelMat;

out vec2 vertex;

const float PI = 3.1415926;
const float SCALE = 14.0f;

void main() {
    gl_Position = modelMat * vec4(gl_in[0].gl_Position.xyz + vec3(1.0f,1.0f,0.0f),1.0);
    vertex= SCALE*vec2(1.0f,1.0f);
    vertex.y -= 1.0;
    EmitVertex();
    
    gl_Position = modelMat * vec4(gl_in[0].gl_Position.xyz + vec3(1.0f,-1.0f,0.0f),1.0);
    vertex= SCALE * vec2(1.0f,-1.0f);
    vertex.y -= 1.0;
    EmitVertex();
    
    gl_Position = modelMat * vec4(gl_in[0].gl_Position.xyz + vec3(-1.0f,-1.0f,0.0f),1.0);
    vertex= SCALE*vec2(-1.0f,-1.0f);
    vertex.y -= 1.0;
    EmitVertex();
    
    gl_Position = modelMat * vec4(gl_in[0].gl_Position.xyz + vec3(-1.0f,-1.0f,0.0f),1.0);
    vertex= SCALE*vec2(-1.0f,-1.0f);
    vertex.y -= 1.0;
    EmitVertex();
    
    gl_Position = modelMat * vec4(gl_in[0].gl_Position.xyz + vec3(-1.0f,1.0f,0.0f),1.0);
    vertex= SCALE*vec2(-1.0f,1.0f);
    vertex.y -= 1.0;
    EmitVertex();
    
    gl_Position = modelMat * vec4(gl_in[0].gl_Position.xyz + vec3(1.0f,1.0f,0.0f),1.0);
    vertex= SCALE*vec2(1.0f,1.0f);
    vertex.y -= 1.0;
    EmitVertex();
    
    EndPrimitive();
}