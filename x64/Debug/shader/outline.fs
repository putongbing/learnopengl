#version 430 core
out vec4 FragColor;

uniform sampler2D tex;
in vec2 outTex;

void main()
{    
	FragColor = texture(tex, outTex);
}