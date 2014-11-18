#version 330

layout (location = 0) in vec3 position;

uniform mat4 WVP;
uniform vec3 cameraPosition;

out vec3 TexCoord0;

void main()
{
    gl_Position = WVP * vec4(cameraPosition  + position*0.5f, 1.0);
    TexCoord0 = position;
}

