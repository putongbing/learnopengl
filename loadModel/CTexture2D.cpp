#include "CTexture2D.h"
#include "glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CTexture2D::CTexture2D() {
	glGenTextures(1, &m_texID);
	glBindTexture(GL_TEXTURE_2D, m_texID);
}

CTexture2D::~CTexture2D() {

}

void CTexture2D::setFilter(unsigned int filterType, unsigned int filter) {
	glTexParameteri(GL_TEXTURE_2D, filterType, filter);
}

void CTexture2D::setWrap(unsigned int wrapType, unsigned int wrap) {
	glTexParameteri(GL_TEXTURE_2D, wrapType, wrap);
}

void CTexture2D::useTexture(unsigned int texUnit) {
	glBindTexture(GL_TEXTURE_2D, m_texID);
	glActiveTexture(texUnit);
}

bool CTexture2D::loadTexture(const char* path, int level, unsigned int pixelType) {
	int width = 0;
	int height = 0;
	int channel = 0;
	unsigned int format = 0;
	std::string texPath = std::string("./nanosuit//") + path;
	stbi_uc* data = stbi_load(texPath.c_str(), &width, &height, &channel, 0);
	if (channel == 1)
		format = GL_RED;
	else if (channel == 3)
		format = GL_RGB;
	else if (channel == 4)
		format = GL_RGBA;
	if (!data) {
		return false;
	}
	glTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, format, pixelType, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//将纹理坐标上下颠倒，和屏幕坐标一样
	stbi_set_flip_vertically_on_load(true);
	stbi_image_free(data);
	return true;
}