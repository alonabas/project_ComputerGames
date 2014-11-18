#version 330

uniform sampler2D textureData;

in vec2 tex_coord;

out vec4 outColor;

void main()
{
    outColor = texture(textureData,tex_coord);

}
