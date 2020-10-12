#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out VS_OUT{
	vec2 outTex;
	vec3 outPos;
	vec3 outNor;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
	vs_out.outTex = tex;
	vs_out.outPos = vec3(vec4(pos, 1.0) * model);
	vs_out.outNor = normal;
	gl_Position = proj * view * model * vec4(pos, 1.0); 
};