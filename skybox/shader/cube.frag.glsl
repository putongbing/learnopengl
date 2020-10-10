#version 430

uniform sampler2D tex;
out vec4 fragColor;
in vec2 outTexCoord;

void main(){
	fragColor = texture(tex, outTexCoord);
}