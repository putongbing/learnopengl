#include "CMesh.h"
#include <glad/glad.h>
#include "stb_image.h"
#include <string>
#include <iostream>

unsigned int TextureFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

std::vector<CMesh::TEST> textures_loaded;

std::vector<CMesh::TEST> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<CMesh::TEST> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		//bool skip = false;
		//for (unsigned int j = 0; j < textures_loaded.size(); j++)
		//{
		//	if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
		//	{
		//		textures.push_back(textures_loaded[j]);
		//		skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
		//		break;
		//	}
		//}
		//if (!skip)
		//{   // if texture hasn't been loaded already, load it
			CMesh::TEST texture;
			std::string dir = "./nanosuit";
			texture.id = TextureFromFile(str.C_Str(), dir);
			texture.m_type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			//textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		//}
	}
	return textures;
};


CMesh::CMesh() {

}

CMesh::~CMesh() {

}

void CMesh::init() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	int sizeVertex = m_vertexInfo.size() * sizeof(VERTEX);
	glBufferData(GL_ARRAY_BUFFER, sizeVertex, &m_vertexInfo[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	int sizeIndex = m_vecIndex.size() * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex, &m_vecIndex[0], GL_STATIC_DRAW);
	int size = sizeof(VERTEX);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)(3 * sizeof(float)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void CMesh::draw(CShader shader) {
	shader.use();
	int diffuseMap = 1;
	int specularMap = 1;
	int normalMap = 1;
	int heightMap = 1;
	for (int i = 0; i < m_vecTexture.size(); i++) {
		TEX texInfo = m_vecTexture[i];
		CTexture2D tex = texInfo.m_tex;
		std::string type = texInfo.m_type;
		std::string name;
		if (type == "texture_diffuse") 
			name = type + std::to_string(diffuseMap++);
		else if(type == "texture_specular")
			name = type + std::to_string(specularMap++);
		else if(type == "texture_normal")
			name = type + std::to_string(normalMap++);
		else if(type == "texture_height")
			name = type + std::to_string(heightMap++);
		shader.setInt(name, i);
		//glBindTexture(GL_TEXTURE_2D, m_vecTexture[i].id);
		//glActiveTexture(GL_TEXTURE0 + i);
		tex.useTexture(GL_TEXTURE0 + i);
	}

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glDrawElements(GL_TRIANGLES, m_vecIndex.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void CMesh::parseMesh(const aiScene* scene, aiMesh* mesh) {
	if (!mesh)
		return;
	int vectexSize = mesh->mNumVertices;
	for (int i = 0; i < vectexSize; i++) {
		//获取顶点坐标
		VERTEX vertex;
		glm::vec3 point;
		point.x = mesh->mVertices[i].x;
		point.y = mesh->mVertices[i].y;
		point.z = mesh->mVertices[i].z;
		vertex.m_vecVertexCoord = point;
		//获取法线坐标
		if (mesh->HasNormals()) {
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.m_vecNormalCoord = normal;
		}
		//获取纹理坐标
		if (mesh->mTextureCoords[0]) {
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.m_vecTexCoord = tex;
		}
		else {
			vertex.m_vecTexCoord = glm::vec2(0.0, 0.0);
		}
		m_vertexInfo.push_back(vertex);
	}
	
	//获取索引缓冲
	int numFaces = mesh->mNumFaces;
	for (int i = 0; i < numFaces; i++) {
		aiFace face = mesh->mFaces[i];
		unsigned int numIndices = face.mNumIndices;
		for (int j = 0; j < numIndices; j++) {
			unsigned int index = face.mIndices[j];
			m_vecIndex.push_back(index);
		}
	}

	//获取材质
	int matIndex = mesh->mMaterialIndex;
	aiMaterial* mat = scene->mMaterials[matIndex];
	//漫反射贴图
	paraseTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	//镜面贴图
	paraseTexture(mat, aiTextureType_SPECULAR, "texture_specular");
	////法线贴图
	//paraseTexture(mat, aiTextureType_NORMALS, "texture_normal");
	////高度贴图
	//paraseTexture(mat, aiTextureType_HEIGHT, "texture_height");
	//std::vector<CMesh::TEST> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	//m_vecTexture.insert(m_vecTexture.end(), diffuseMaps.begin(), diffuseMaps.end());
	//// 2. specular maps
	//std::vector<CMesh::TEST> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
	//m_vecTexture.insert(m_vecTexture.end(), specularMaps.begin(), specularMaps.end());
	init();
}

void CMesh::paraseTexture(aiMaterial* mat, aiTextureType texType, std::string type) {
	//漫反射贴图
	int size = mat->GetTextureCount(texType);
	for (int i = 0; i < size; i++) {
		aiString str;
		mat->GetTexture(texType, i, &str);
		CTexture2D tex;
		tex.loadTexture(str.C_Str(), 0, UNSIGNED_BYTE);
		TEX texInfo;
		texInfo.m_tex = tex;
		texInfo.m_type = type;
		m_vecTexture.push_back(texInfo);
	}
}