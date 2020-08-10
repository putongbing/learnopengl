#version 330 core								
layout (location = 0) in vec3 pos;	
layout (location = 1) in vec3 nor;

out vec3 outNor;
out vec3 outPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){									
	gl_Position = proj * view * model * vec4(pos, 1.0);
	outNor = mat3(transpose(inverse(model))) * nor;
	outPos = mat3(model) * pos;
}	