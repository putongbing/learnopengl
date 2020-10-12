#version 430

layout(location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 proj;

out vec3 outTexCoord;

void main(){
	outTexCoord = pos;
	vec4 outPos = proj * view * vec4(pos, 1.0);
	gl_Position = outPos.xyww;
}