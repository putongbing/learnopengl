#version 330 core	
in vec3 outNor;
in vec3 outPos;
in vec2 outTex;
out vec4 fragColor;

struct light{
	//灯光位置
	vec3 pos;
	//环境光
	vec3 ambient;
	//漫反射
	vec3 diffuse;
	//镜面反射
	vec3 specular;
};

struct material{
	//近似将环境光等于漫反射光
	sampler2D diffuse;
	sampler2D specular;
	//反光度
	float reflectance;
};

uniform light	lightInfo;
uniform material materialInfo;
//视点位置
uniform vec3 viewPos;

void main() {
	vec3 lightDir = normalize(lightInfo.pos - outPos);
	//环境光
	vec3 ambient = lightInfo.ambient * texture(materialInfo.diffuse, outTex).rgb;
	//漫反射光
	float diff = max(dot(lightDir, outNor), 0.0);
	vec3 diffuse = diff * lightInfo.diffuse * texture(materialInfo.diffuse, outTex).rgb;
	//镜面反射
	vec3 ref = normalize(reflect(-lightDir, outNor));
	vec3 viewDir = normalize(viewPos - outPos);
	float spe = pow(max(dot(ref, viewDir), 0.0), materialInfo.reflectance);
	vec3 specular = spe * lightInfo.specular * texture(materialInfo.specular, outTex).rgb;
	fragColor = vec4(ambient + diffuse + specular, 1.0);
}				