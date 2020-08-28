#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec2 outTex;
out vec3 outPos;
out vec3 outNor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
	outTex = tex;
	outPos = vec3(vec4(pos, 1.0) * model);
	outNor = normal;
	gl_Position = proj * view * model * vec4(pos, 1.0); 
};