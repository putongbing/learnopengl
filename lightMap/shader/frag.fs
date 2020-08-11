#version 330 core	
in vec3 outNor;
in vec3 outPos;
in vec2 outTex;
out vec4 fragColor;

struct light{
	//�ƹ�λ��
	vec3 pos;
	//������
	vec3 ambient;
	//������
	vec3 diffuse;
	//���淴��
	vec3 specular;
};

struct material{
	//���ƽ�����������������
	sampler2D diffuse;
	sampler2D specular;
	//�����
	float reflectance;
};

uniform light	lightInfo;
uniform material materialInfo;
//�ӵ�λ��
uniform vec3 viewPos;

void main() {
	vec3 lightDir = normalize(lightInfo.pos - outPos);
	//������
	vec3 ambient = lightInfo.ambient * texture(materialInfo.diffuse, outTex).rgb;
	//�������
	float diff = max(dot(lightDir, outNor), 0.0);
	vec3 diffuse = diff * lightInfo.diffuse * texture(materialInfo.diffuse, outTex).rgb;
	//���淴��
	vec3 ref = normalize(reflect(-lightDir, outNor));
	vec3 viewDir = normalize(viewPos - outPos);
	float spe = pow(max(dot(ref, viewDir), 0.0), materialInfo.reflectance);
	vec3 specular = spe * lightInfo.specular * texture(materialInfo.specular, outTex).rgb;
	fragColor = vec4(ambient + diffuse + specular, 1.0);
}				