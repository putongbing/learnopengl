#pragma once

#include <string>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>

class CMesh;
class CShader;

class CModelX
{
public:
	CModelX();
	~CModelX();
	/*
	loadModel:����ģ��
	param-filePath:ģ���ļ�·��
	*/
	bool loadModel(std::string filePath);
	/*
	draw:����ģ��
	param-shader:����ģ��ʹ�õ���ɫ��
	*/
	void draw(CShader shader);
	/*
	traverseNode:�ݹ����ģ���еĽڵ�
	param-scene:ģ�ͳ���
	param-node:��ǰ�ڵ�
	*/
	void traverseNode(const aiScene* scene, aiNode* node);
	/*
	traverseMesh:����ģ�ͽڵ��е�mesh
	param-scene:ģ�ͳ���
	param-node:��ǰ�ڵ�
	*/
	void traverseMesh(const aiScene* scene, unsigned int meshIndex);
private:
	std::vector<CMesh> m_vecMesh;
};

