#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
	gl_Position = proj * view * model * vec4(pos, 1.0); 
};