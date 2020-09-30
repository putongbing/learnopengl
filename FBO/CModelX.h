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
	loadModel:加载模型
	param-filePath:模型文件路径
	*/
	bool loadModel(std::string filePath);
	/*
	draw:绘制模型
	param-shader:绘制模型使用的着色器
	*/
	void draw(CShader shader);
	/*
	traverseNode:递归遍历模型中的节点
	param-scene:模型场景
	param-node:当前节点
	*/
	void traverseNode(const aiScene* scene, aiNode* node);
	/*
	traverseMesh:遍历模型节点中的mesh
	param-scene:模型场景
	param-node:当前节点
	*/
	void traverseMesh(const aiScene* scene, unsigned int meshIndex);
private:
	std::vector<CMesh> m_vecMesh;
};

