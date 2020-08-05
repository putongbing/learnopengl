#version 330 core	
in vec4 outColor;
in vec2 outTex;
out vec4 fragColor;	
uniform sampler2D wallTex;
uniform sampler2D smileTex;
void main() {							
	fragColor = mix(texture(wallTex, outTex), texture(smileTex, outTex), 0.2);
}				