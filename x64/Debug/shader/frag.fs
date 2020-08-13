#version 330 core	
in vec3 outNor;
in vec3 outPos;
in vec2 outTex;
out vec4 fragColor;

//定向光
struct dirLight{
	vec3 dir;
	//环境光
	vec3 ambient;
	//漫反射
	vec3 diffuse;
	//镜面反射
	vec3 specular;
};

//点光源
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

//聚光
struct spotLight{
	//光的位置
	vec3 pos;
	//聚光方向向量
	vec3 slDir;
	//内角余弦值
	float innerAngle;
	//外角余弦值
	float outAngle;
	//环境光
	vec3 ambient;
	//漫反射
	vec3 diffuse;
	//镜面反射
	vec3 specular;
};

//衰减常量
struct attenuation{
	//常数项
	float constant;
	//一次向
	float liner;
	//二次项
	float quadratic;
};

struct material{
	//近似将环境光等于漫反射光
	sampler2D diffuse;
	sampler2D specular;
	//反光度
	float reflectance;
};

uniform light	lightInfo[4];
uniform dirLight dlInfo;
uniform spotLight slInfo;
uniform material materialInfo;
uniform attenuation atte;
//视点位置
uniform vec3 viewPos;

//计算点光源
vec3 calcPotLight(){
	vec3 allLight;
	for(int i = 0; i < 4; i++){
		//环境光
		vec3 ambient = lightInfo[i].ambient * texture(materialInfo.diffuse, outTex).rgb;
		//漫反射
		vec3 lightDir = normalize(lightInfo[i].pos - outPos);
		float diff = max(dot(lightDir, outNor), 0);
		vec3 diffuse = diff * lightInfo[i].diffuse * texture(materialInfo.diffuse, outTex).rgb;
		//镜面反射
		vec3 ref = reflect(-lightDir, outNor);
		vec3 viewDir = normalize(viewPos - outPos);
		float spec = pow(max(dot(ref, viewDir), 0.0) ,materialInfo.reflectance);
		vec3 specular = spec * lightInfo[i].specular * texture(materialInfo.specular, outTex).rgb;
		//计算衰减系数
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

//计算聚光
vec3 calcSpotLight(){
	//环境光
	vec3 ambient = slInfo.ambient * texture(materialInfo.diffuse, outTex).rgb;
	//漫反射
	vec3 lightDir = normalize(slInfo.pos - outPos);
	float diff = max(dot(lightDir, outNor), 0);
	vec3 diffuse = diff * slInfo.diffuse * texture(materialInfo.diffuse, outTex).rgb;
	//镜面反射
	vec3 ref = reflect(-lightDir, outNor);
	vec3 viewDir = normalize(viewPos - outPos);
	float spec = pow(max(dot(ref, viewDir), 0.0) ,materialInfo.reflectance);
	vec3 specular = spec * slInfo.specular * texture(materialInfo.specular, outTex).rgb;
	//计算衰减系数
	float dis = distance(slInfo.pos, outPos);
	float atte = 1.0 / (atte.constant + atte.liner * dis + atte.quadratic * dis * dis);
	ambient *= atte;
	diffuse *= atte;
	specular *= atte;
	//计算柔化系数
	//光线与聚光方向向量的夹角
	float spotAngle = dot(lightDir, normalize(-slInfo.slDir));
	float spot = (spotAngle - slInfo.outAngle)/ (slInfo.innerAngle - slInfo.outAngle);
	spot = clamp(spot, 0.0, 1.0);
	ambient *= spot;
	diffuse *= spot;
	specular *= spot;
	return ambient + diffuse + specular;
}

//计算定向光
vec3 calcDirLight(){
	//环境光
	vec3 ambient = dlInfo.ambient * texture(materialInfo.diffuse, outTex).rgb;
	//漫反射
	vec3 lightDir = normalize(-dlInfo.dir);
	float diff = max(dot(lightDir, outNor), 0);
	vec3 diffuse = diff * dlInfo.diffuse * texture(materialInfo.diffuse, outTex).rgb;
	//镜面反射
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