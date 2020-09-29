#include "CModelX.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "CMesh.h"
#include "CShader.h"

CModelX::CModelX() {

}

CModelX::~CModelX() {
	
}

bool CModelX::loadModel(std::string filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene)
		return false;
	//获取根节点
	aiNode* rootNode = scene->mRootNode;
	if (!rootNode)
		return false;
	traverseNode(scene, rootNode);
	return true;
}

void CModelX::traverseNode(const aiScene* scene, aiNode* node) {
	if (!node)
		return;
	int numMeshs = node->mNumMeshes;
	for (int j = 0; j < numMeshs; j++) {
		traverseMesh(scene, node->mMeshes[j]);
	}
	int numChildren = node->mNumChildren;
	for (int i = 0; i < numChildren; i++) {
		aiNode* subNode = node->mChildren[i];
		traverseNode(scene, subNode);
	}
}

void CModelX::traverseMesh(const aiScene* scene, unsigned int meshIndex) {
	aiMesh* mesh = scene->mMeshes[meshIndex];
	CMesh customMesh;
	customMesh.parseMesh(scene, mesh);
	m_vecMesh.push_back(customMesh);
}

void CModelX::draw(CShader shader) {
	for (int i = 0; i < m_vecMesh.size(); i++) {
		m_vecMesh[i].draw(shader);
	}
}