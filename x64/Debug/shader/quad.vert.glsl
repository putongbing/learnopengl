#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

out vec2 outTexCoord;

void main(){
	outTexCoord = texCoord;
	gl_Position = vec4(pos, 1.0);
}