#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
uniform vec2 resolution;
void main(){
    
	// Output position of the vertex, in clip space
    vec2 normalization = resolution/2.0;
	vec2 vertexPosition_homoneneousspace = position - normalization; 
	vertexPosition_homoneneousspace /= normalization;
	gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
	
	// UV of the vertex. No special space for this one.
	UV = texCoord;
}

