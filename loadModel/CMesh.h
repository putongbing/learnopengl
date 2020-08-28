#pragma once

#include <vector>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include "../glm/glm.hpp"
#include <assimp/scene.h>
#include "CTexture2D.h"
#include "CShader.h"

class CMesh
{
public:
	struct TEX {
		CTexture2D	m_tex;
		std::string	m_type;
	};
	struct TEST {
		unsigned int id;
		std::string m_type;
		std::string path;
	};
	struct VERTEX {
		//mesh vertex
		glm::vec3		m_vecVertexCoord;
		//mesh normal
		glm::vec3		m_vecNormalCoord;
		//mesh texture
		glm::vec2		m_vecTexCoord;
	};
	CMesh();
	~CMesh();
	void init();
	void draw(CShader shader);
	void parseMesh(const aiScene* scene, aiMesh* mesh);
	void paraseTexture(aiMaterial* mat, aiTextureType texType, std::string type);
private:
	//mesh index
	std::vector<unsigned int>	m_vecIndex;
	std::vector<VERTEX>			m_vertexInfo;
	//mesh texture
	std::vector<TEX>			m_vecTexture;
	unsigned int				m_vao;
	unsigned int				m_vbo;
	unsigned int				m_ebo;
 };

