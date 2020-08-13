#version 330 core	
in vec3 outNor;
in vec3 outPos;
in vec2 outTex;
out vec4 fragColor;

//�����
struct dirLight{
	vec3 dir;
	//������
	vec3 ambient;
	//������
	vec3 diffuse;
	//���淴��
	vec3 specular;
};

//���Դ
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

//�۹�
struct spotLight{
	//���λ��
	vec3 pos;
	//�۹ⷽ������
	vec3 slDir;
	//�ڽ�����ֵ
	float innerAngle;
	//�������ֵ
	float outAngle;
	//������
	vec3 ambient;
	//������
	vec3 diffuse;
	//���淴��
	vec3 specular;
};

//˥������
struct attenuation{
	//������
	float constant;
	//һ����
	float liner;
	//������
	float quadratic;
};

struct material{
	//���ƽ�����������������
	sampler2D diffuse;
	sampler2D specular;
	//�����
	float reflectance;
};

uniform light	lightInfo[4];
uniform dirLight dlInfo;
uniform spotLight slInfo;
uniform material materialInfo;
uniform attenuation atte;
//�ӵ�λ��
uniform vec3 viewPos;

//������Դ
vec3 calcPotLight(){
	vec3 allLight;
	for(int i = 0; i < 4; i++){
		//������
		vec3 ambient = lightInfo[i].ambient * texture(materialInfo.diffuse, outTex).rgb;
		//������
		vec3 lightDir = normalize(lightInfo[i].pos - outPos);
		float diff = max(dot(lightDir, outNor), 0);
		vec3 diffuse = diff * lightInfo[i].diffuse * texture(materialInfo.diffuse, outTex).rgb;
		//���淴��
		vec3 ref = reflect(-lightDir, outNor);
		vec3 viewDir = normalize(viewPos - outPos);
		float spec = pow(max(dot(ref, viewDir), 0.0) ,materialInfo.reflectance);
		vec3 specular = spec * lightInfo[i].specular * texture(materialInfo.specular, outTex).rgb;
		//����˥��ϵ��
		float dis = distance(lightInfo[i].pos, outPos);
		float atte = 1.0 / (atte.constant + atte.liner * dis + atte.quadratic * dis * dis);
		ambient *= atte;
		diffuse *= atte;
		specular *= atte;
		vec3 light = ambient + diffuse + specular;
		allLight += light;
	}
	return allLight;
}

//����۹�
vec3 calcSpotLight(){
	//������
	vec3 ambient = slInfo.ambient * texture(materialInfo.diffuse, outTex).rgb;
	//������
	vec3 lightDir = normalize(slInfo.pos - outPos);
	float diff = max(dot(lightDir, outNor), 0);
	vec3 diffuse = diff * slInfo.diffuse * texture(materialInfo.diffuse, outTex).rgb;
	//���淴��
	vec3 ref = reflect(-lightDir, outNor);
	vec3 viewDir = normalize(viewPos - outPos);
	float spec = pow(max(dot(ref, viewDir), 0.0) ,materialInfo.reflectance);
	vec3 specular = spec * slInfo.specular * texture(materialInfo.specular, outTex).rgb;
	//����˥��ϵ��
	float dis = distance(slInfo.pos, outPos);
	float atte = 1.0 / (atte.constant + atte.liner * dis + atte.quadratic * dis * dis);
	ambient *= atte;
	diffuse *= atte;
	specular *= atte;
	//�����ữϵ��
	//������۹ⷽ�������ļн�
	float spotAngle = dot(lightDir, normalize(-slInfo.slDir));
	float spot = (spotAngle - slInfo.outAngle)/ (slInfo.innerAngle - slInfo.outAngle);
	spot = clamp(spot, 0.0, 1.0);
	ambient *= spot;
	diffuse *= spot;
	specular *= spot;
	return ambient + diffuse + specular;
}

//���㶨���
vec3 calcDirLight(){
	//������
	vec3 ambient = dlInfo.ambient * texture(materialInfo.diffuse, outTex).rgb;
	//������
	vec3 lightDir = normalize(-dlInfo.dir);
	float diff = max(dot(lightDir, outNor), 0);
	vec3 diffuse = diff * dlInfo.diffuse * texture(materialInfo.diffuse, outTex).rgb;
	//���淴��
	vec3 ref = reflect(-lightDir, outNor);
	vec3 viewDir = normalize(viewPos - outPos);
	float spec = pow(max(dot(ref, viewDir), 0.0) ,materialInfo.reflectance);
	vec3 specular = spec * dlInfo.specular * texture(materialInfo.specular, outTex).rgb;
	return ambient + diffuse + specular;
}

void main() {
	vec3 pot = calcPotLight();
	vec3 spot = calcSpotLight();
	vec3 dir = calcDirLight();
	fragColor = vec4(dir + spot + pot, 1.0);
}				