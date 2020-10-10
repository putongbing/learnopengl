#version 430

uniform samplerCube tex;
out vec4 fragColor;
in vec3 outTexCoord;

void main(){
	fragColor = texture(tex, -outTexCoord);
}