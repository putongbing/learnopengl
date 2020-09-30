#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 outTexCoord;

void main(){
	outTexCoord = texCoord;
	gl_Position = proj * view * model * vec4(pos, 1.0);
}