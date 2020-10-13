#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;
layout (location = 3) in mat4 model;

out vec2 outTex;

uniform mat4 view;
uniform mat4 proj;

void main(){
	outTex = tex;
	gl_Position = proj * view * model * vec4(pos, 1.0); 
};