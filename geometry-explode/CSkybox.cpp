#include "CSkybox.h"
#include "stb_image.h"
#include <iostream>

float CSkybox::m_skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

CSkybox::CSkybox(CCamera& camera) {
	m_camera = &camera;
	m_paths = {
		"./skybox-image//right.jpg",
		"./skybox-image//left.jpg",
		"./skybox-image//bottom.jpg",
		"./skybox-image//top.jpg",
		"./skybox-image//front.jpg",
		"./skybox-image//back.jpg"
	};
	init();
	loadImage();
}

CSkybox::~CSkybox() {

}

void CSkybox::init() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), m_skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	unsigned int sbVertId = m_shader.createShader(CShader::VERTEX);
	bool ret = m_shader.compileShaderByFile(sbVertId, "./shader//sb.vert.glsl");
	if (ret != 1) {
		std::cout << "Failed to load shader that skybox vertex shader!" << std::endl;
		return;
	}
		
	unsigned int sbFragId = m_shader.createShader(CShader::FRAGMENT);
	ret = m_shader.compileShaderByFile(sbFragId, "./shader//sb.frag.glsl");
	if (ret != 1) {
		std::cout << "Failed to load shader that skybox fragment shader!" << std::endl;
		return;
	}
	unsigned int sbLstId[] = { sbVertId, sbFragId };
	ret = m_shader.linkShader(2, sbLstId);
	if (ret != 1) {
		std::cout << "Failed to load shader that skybox link!" << std::endl;
	}
}

void CSkybox::use() {
	glm::mat4 view = m_camera->getViewMat();
	glm::mat4 proj = m_camera->getProjectionMat();

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(m_vao);
	m_shader.use();
	view = glm::mat4(glm::mat3(view));
	m_shader.setMatrix4x4("view", view);
	m_shader.setMatrix4x4("proj", proj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texUnit);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void CSkybox::loadImage() {
	glGenTextures(1, &m_texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texUnit);
	unsigned int format = 0;
	for (int i = 0; i < m_paths.size(); i++) {
		int width, height, comp;
		const char* path = m_paths[i].c_str();
		unsigned char* data = stbi_load(path, &width, &height, &comp, 0);
		if (comp == 1) {
			format = GL_RED;
		}
		else if (comp == 3) {
			format = GL_RGB;
		}
		else if (comp == 4) {
			format == GL_RGBA;
		}
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture!" << std::endl;
			return;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}