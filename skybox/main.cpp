// window.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"
#include "common.h"
#include "CTexture2D.h"
#include "../glm/gtc/matrix_transform.hpp"
#include "CCamera.h"
#include "CModelX.h"
#include "./stb_image.h"
#include "CSkybox.h"

float lastX = 0.0;
float lastY = 0.0;
bool  firstFrame = true;
float currentFrame = 0.0;
float deltaTime = 0.0;
float lastFrame = 0.0;
CCamera camera;

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

std::vector<std::string> paths{
	"./skybox-image//right.jpg",
	"./skybox-image//left.jpg",
	"./skybox-image//bottom.jpg",
	"./skybox-image//top.jpg",
	"./skybox-image//front.jpg",
	"./skybox-image//back.jpg"
};

float skyboxVertices[] = {
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


GLuint loadSkybox() {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	unsigned int format = 0;
	for (int i = 0; i < paths.size(); i++) {
		int width, height, comp;
		const char* path = paths[i].c_str();
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
			return -1;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}



void input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.down(deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.up(deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.right(deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.left(deltaTime);	
}

void cursorPos(GLFWwindow* window, double x, double y) {
	if (firstFrame) {
		lastX = x;
		lastY = y;
		firstFrame = false;
	}
	float dX = x - lastX;
	float dY = y - lastY;
	lastX = x;
	lastY = y;
	camera.adjustPitchYaw(dX, dY);
}

float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


int main()
{
	glfwInit();
	//use opengl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//use opengl core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create window
	int winWidth = 800;
	int winHeight = 600;
	char title[256] = "learnopengl";
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, title, nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwSetFramebufferSizeCallback(window, windowSizeCallback);
	//设置光标位置回调
	glfwSetCursorPosCallback(window, cursorPos);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}
	glViewport(0, 0, winWidth, winHeight);

	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	CTexture2D cubeTex;
	cubeTex.loadTexture("./image//smile.png", 0, UNSIGNED_BYTE);
	CShader cubeShader;
	unsigned int cubeVertId = cubeShader.createShader(CShader::VERTEX);
	bool ret = cubeShader.compileShaderByFile(cubeVertId, "./shader//cube.vert.glsl");
	if (ret != 1)
		return 0;
	unsigned int cubeFragId = cubeShader.createShader(CShader::FRAGMENT);
	ret = cubeShader.compileShaderByFile(cubeFragId, "./shader//cube.frag.glsl");
	if (ret != 1) {
		char err[1024] = "";
		cubeShader.getCompileOrLinkInfo(cubeFragId, CShader::COMPILE, err);
		return 0;
	}
	unsigned int lstId[] = { cubeVertId, cubeFragId };
	ret = cubeShader.linkShader(2, lstId);
	if (ret != 1)
		return 0;

	glm::mat4 proj = glm::perspective(45.0, winWidth / (double)winHeight, 1.0, 1000.0);
	camera.setProjectionMat(proj);
	CSkybox sb(camera);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		if (firstFrame)
			lastFrame = currentFrame;
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		input(window);

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0);
		glm::mat4 view = camera.getViewMat();

		sb.use();

		glBindVertexArray(cubeVAO);
		cubeShader.use();
		view = camera.getViewMat();
		cubeShader.setMatrix4x4("model", model);
		cubeShader.setMatrix4x4("view", view);
		cubeShader.setMatrix4x4("proj", proj);
		cubeTex.useTexture(TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


