#version 330

layout (points) in;

layout (triangle_strip) out;
layout (max_vertices = 44) out;

uniform mat4 scale;
uniform mat4 translate;
out vec2 vertex;

void main() {
    gl_Position = translate * scale * vec4(gl_in[0].gl_Position.xyz + vec3(1.0f,1.0f,0.0f),1.0);
    vertex = vec2(1.0f,0.0f);
    EmitVertex();
    
    gl_Position = translate * scale * vec4(gl_in[0].gl_Position.xyz + vec3(1.0f,-1.0f,0.0f),1.0);
    vertex = vec2(1.0f,1.0f);
    EmitVertex();
    
    gl_Position =  translate * scale * vec4(gl_in[0].gl_Position.xyz + vec3(-1.0f,-1.0f,0.0f),1.0);
    vertex = vec2(0.0f,1.0f);
    EmitVertex();
    
    gl_Position = translate * scale * vec4(gl_in[0].gl_Position.xyz + vec3(-1.0f,-1.0f,0.0f),1.0);
    vertex = vec2(0.0f,1.0f);
    EmitVertex();
    
    gl_Position = translate * scale* vec4(gl_in[0].gl_Position.xyz + vec3(-1.0f,1.0f,0.0f),1.0);
    vertex = vec2(0.0f,0.0f);
    EmitVertex();
    
    gl_Position = translate * scale * vec4(gl_in[0].gl_Position.xyz + vec3(1.0f,1.0f,0.0f),1.0);
    vertex = vec2(1.0f,0.0f);
    EmitVertex();
    
    EndPrimitive();
}