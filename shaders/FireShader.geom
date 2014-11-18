#version 330

layout (points) in;

layout (triangle_strip) out;
layout (max_vertices = 45) out;
uniform vec3 cameraTarget;

in vec3 col[];

out vec3 color;

//out vec3 color;
void main() {
    gl_Position = vec4(cameraTarget,0.0f) + gl_in[0].gl_Position + vec4(0.005,0.005,0.0,0.0);
    color = col[0];
    EmitVertex();
    gl_Position = vec4(cameraTarget,0.0f) + gl_in[0].gl_Position + vec4(0.005,-0.005,0.0,0.0);
    color = col[0];
    EmitVertex();
    gl_Position = vec4(cameraTarget,0.0f) + gl_in[0].gl_Position + vec4(-0.005,-0.005,0.0,0.0);
    color = col[0];
    EmitVertex();
    gl_Position = vec4(cameraTarget,0.0f) + gl_in[0].gl_Position + vec4(-0.005,-0.005,0.0,0.0);
    color = col[0];
    EmitVertex();
    gl_Position = vec4(cameraTarget,0.0f) + gl_in[0].gl_Position + vec4(-0.005,0.005,0.0,0.0);
    color = col[0];
    EmitVertex();
    gl_Position = vec4(cameraTarget,0.0f) + gl_in[0].gl_Position + vec4(0.005,0.005,0.0,0.0);
    color = col[0];
    EmitVertex();
    EndPrimitive();
}