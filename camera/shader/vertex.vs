#version 330 core								
layout (location = 0) in vec3 pos;	
layout (location = 1) in vec2 tex;
out vec2 outTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main(){									
	gl_Position = proj * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
	outTex = tex;
}	