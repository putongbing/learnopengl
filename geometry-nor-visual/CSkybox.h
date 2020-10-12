#pragma once

#include "CCamera.h"
#include <vector>
#include <string>
#include "glad.h"
#include "CShader.h"

//need first draw and opengl depth test
class CSkybox
{
public:
	CSkybox(CCamera& camera);
	~CSkybox();
	void use();
protected:
	void loadImage();
	void init();
private:
	CCamera*					m_camera;
	std::vector<std::string>	m_paths;
	static float				m_skyboxVertices[108];
	GLuint						m_texUnit;
	GLuint						m_vao;
	GLuint						m_vbo;
	CShader						m_shader;
};

