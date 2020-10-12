#version 430 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 pos;
in vec3 nor;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{    
	vec3 lightDir = normalize(lightPos - pos);
	vec3 viewDir = normalize(viewPos - pos);
	float diff = max(dot(lightDir, nor), 0.0);
	vec3 diffuse = diff * lightColor * texture(texture_diffuse1, texCoord).rgb;
	//∑¥…‰π‚œﬂ
	vec3 ref = reflect(-lightDir, nor);
	float spe = pow(max(dot(ref, viewDir), 0.0), 32.0);
	vec3 specular = spe * lightColor * texture(texture_specular1, texCoord).rgb;
    FragColor = vec4(specular + diffuse, 1.0);
}