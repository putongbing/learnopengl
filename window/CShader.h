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
	createShader��������ɫ��
	param-type��������ɫ������
	return����ɫ��id,���ط���ֵ�����ɹ�����֮����
	*/
	unsigned int createShader(SHADER_TYPE type);
	/*
	compileShader:����ɫ�����븽�ӵ���ɫ��������
	param-shderID:��ɫ��id
	param-shaderCode:��ɫ������
	return:����ɹ�����ֵ����0������С��0
	*/
	int	compileShader(GLuint shaderID, const char* shaderCode);
	/*
	compileShaderByFile:���ļ��ж�����ɫ�����븽�ӵ���ɫ��������
	param-shderID:��ɫ��id
	param-filePath:��ɫ�������ļ�·��
	return:����ɹ�����ֵ1�����򷵻�0
	*/
	int	compileShaderByFile(GLuint shaderID, const char* filePath);
	/*
	getCompileOrLinkInfo:��ȡ��ɫ���ı�������Ӻ����Ϣ
	param-shderID:��ɫ��id
	return:��ɫ��������Ϣ
	*/
	char* getCompileOrLinkInfo(GLuint id, SHADER_STAUS status);
	/*
	linkShader:����ɫ����������
	param-size:��Ҫ���ӵ���ɫ������
	param-arrShaderID:��ɫ����ID����
	return:���ӳɹ�����1�����򷵻�0
	*/
	int linkShader(int size, unsigned int arrShaderID[]);
	/*
	uniformError:����uniform��������ʱ�����Ϣ
	*/
	void uniformError();
	/*������ɫ���и������͵�uniform����*/
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

