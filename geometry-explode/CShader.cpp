#include "CShader.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

CShader::CShader() {
	m_programID = glCreateProgram();
}

CShader::~CShader() {

}

unsigned int CShader::createShader(SHADER_TYPE type) {
	unsigned int shderID = 0;
	switch (type) {
	case  VERTEX: {
		shderID = glCreateShader(GL_VERTEX_SHADER);
	}
	break;
	case GEOMETRY: {
		shderID = glCreateShader(GL_GEOMETRY_SHADER);
	}
	break;
	case FRAGMENT: {
		shderID = glCreateShader(GL_FRAGMENT_SHADER);
	}
	break;
	default:
		break;
	}
	return shderID;
}

int	CShader::compileShader(GLuint shaderID, const char* shaderCode) {
	int success = -1;
	glShaderSource(shaderID, 1, &shaderCode, nullptr);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	return success;
}

int	CShader::compileShaderByFile(GLuint shaderID, const char* filePath) {
	std::ifstream ifs;
	std::stringstream ss;
	std::string shaderCode;
	try {
		ifs.open(filePath);
		ss << ifs.rdbuf();
		shaderCode = ss.str();
		return compileShader(shaderID, shaderCode.c_str());
	}
	catch (std::ifstream::failure e) {
		std::cout << "Fail to read shader file" << std::endl;
	}
	return -1;
}

void CShader::getCompileOrLinkInfo(GLuint id, SHADER_STAUS status, char* error) {
	GLsizei length;
	if (status == COMPILE)
		glGetShaderInfoLog(id, 1024, &length, error);
	else
		glGetProgramInfoLog(id, 1024, &length, error);
}

int CShader::linkShader(int size, unsigned int arrShaderID[]) {
	int ret = -1;
	if (size < 0)
		return ret;
	for (int i = 0; i < size; i++) {
		glAttachShader(m_programID, arrShaderID[i]);
	}
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &ret);
	return ret;
}

bool CShader::setBool(std::string name, bool value) {
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniform1i(location, value);
	return true;
}

bool CShader::setInt(std::string name, int value) {
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniform1i(location, value);
	return true;
}

bool CShader::setFloat(std::string name, float value) {
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniform1f(location, value);
	return true;
}

bool CShader::setVec2(std::string name, glm::vec2 value) {
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniform2f(location, value[0], value[1]);
	return true;
}

bool CShader::setVec3(std::string name, glm::vec3 value){
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniform3f(location, value[0], value[1], value[2]);
	return true;
}

bool CShader::setVec4(std::string name, glm::vec4 value) {
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniform4f(location, value[0], value[1], value[2], value[3]);
	return true;
}

bool CShader::setMatrix4x4(std::string name, glm::mat4 value) {
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1) {
		uniformError();
		return false;
	}
	glUniformMatrix4fv(location, 1, false, &value[0][0]);
	return true;
}

void CShader::uniformError() {
	std::cout << "Fail to find uniform variable!" << std::endl;
}

void CShader::use() {
	glUseProgram(m_programID);
}