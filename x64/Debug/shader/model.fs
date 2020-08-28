#version 330 core
out vec4 FragColor;

in vec2 outTex;
in vec3 outPos;
in vec3 outNor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{    
	vec3 lightDir = normalize(lightPos - outPos);
	vec3 viewDir = normalize(viewPos - outPos);
	float diff = max(dot(lightDir, outNor), 0.0);
	vec3 diffuse = diff * lightColor * texture(texture_diffuse1, outTex).rgb;
	//∑¥…‰π‚œﬂ
	vec3 ref = reflect(-lightDir, outNor);
	float spe = pow(max(dot(ref, viewDir), 0.0), 32.0);
	vec3 specular = spe * lightColor * texture(texture_specular1, outTex).rgb;
    FragColor = vec4(diffuse + specular, 1.0);
}