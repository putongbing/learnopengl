#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

class CShader
{
public:
	CShader();
	~CShader();
	//shader type
	enum SHADER_TYPE {
		//vertex shader
		VERTEX,
		//geometry shader
		GEOMETRY,
		//fragment shader
		FRAGMENT
	};
	enum SHADER_STAUS{
		//compile
		COMPILE,
		//link
		LINK
	};
	/*
	createShader：创建着色器
	param-type：创建着色器类型
	return：着色器id,返回非零值创建成功，反之成立
	*/
	unsigned int createShader(SHADER_TYPE type);
	/*
	compileShader:将着色器代码附加到着色器并编译
	param-shderID:着色器id
	param-shaderCode:着色器代码
	return:编译成功返回值大于0，否则小于0
	*/
	int	compileShader(GLuint shaderID, const char* shaderCode);
	/*
	compileShaderByFile:从文件中读出着色器代码附加到着色器并编译
	param-shderID:着色器id
	param-filePath:着色器代码文件路径
	return:编译成功返回值1，否则返回0
	*/
	int	compileShaderByFile(GLuint shaderID, const char* filePath);
	/*
	getCompileOrLinkInfo:获取着色器的编译或链接后的信息
	param-shderID:着色器id
	return:着色器编译信息
	*/
	char* getCompileOrLinkInfo(GLuint id, SHADER_STAUS status);
	/*
	linkShader:将着色器进行链接
	param-size:需要链接的着色器个数
	param-arrShaderID:着色器的ID数组
	return:链接成功返回1，否则返回0
	*/
	int linkShader(int size, unsigned int arrShaderID[]);
	/*
	uniformError:设置uniform变量错误时输出信息
	*/
	void uniformError();
	/*设置着色器中各种类型的uniform变量*/
	bool setBool(std::string name, bool value);
	bool setInt(std::string name, int value);
	bool setFloat(std::string name, float value);
	bool setVec2(std::string name, std::vector<float> value);
	bool setVec3(std::string name, std::vector<float> value);
	bool setVec4(std::string name, std::vector<float> value);
	bool setMatrix4x4(std::string name, std::vector<float> value);
	void use();
private:
	GLuint m_programID;
};

