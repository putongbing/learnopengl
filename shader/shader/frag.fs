#version 330 core	
in vec4 outColor;
out vec4 fragColor;	
uniform vec4 ufColor;
void main() {							
	fragColor = ufColor;
}				