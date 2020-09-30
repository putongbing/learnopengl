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

float planeVertices[] = {
	// positions          // texture Coords 
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

float quadVertices[] = { 
	// positions   // texCoords
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
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
	//glfwSetCursorPosCallback(window, cursorPos);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}
	glViewport(0, 0, winWidth, winHeight);

	//generate fbo
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//generate texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	
	GLuint renderObj;
	glGenRenderbuffers(1, &renderObj);
	glBindRenderbuffer(GL_RENDERBUFFER, renderObj);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObj);

	GLenum ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (ret != GL_FRAMEBUFFER_COMPLETE)
		return 0;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	CTexture2D cubeTex;
	cubeTex.loadTexture("./image//smile.png", 0, UNSIGNED_BYTE);
	CTexture2D planeTex;
	planeTex.loadTexture("./image//wall.jpg", 0, UNSIGNED_BYTE);

	CShader cubeShader;
	unsigned int cubeVertId = cubeShader.createShader(CShader::VERTEX);
	ret = cubeShader.compileShaderByFile(cubeVertId, "./shader//cube.vert.glsl");
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
	CShader planeShader;
	unsigned int planeVertId = cubeShader.createShader(CShader::VERTEX);
	ret = planeShader.compileShaderByFile(planeVertId, "./shader//plane.vert.glsl");
	if (ret != 1)
		return 0;
	unsigned int planeFragId = cubeShader.createShader(CShader::FRAGMENT);
	ret = planeShader.compileShaderByFile(planeFragId, "./shader//plane.frag.glsl");
	if (ret != 1) {
		char err[1024] = "";
		planeShader.getCompileOrLinkInfo(planeFragId, CShader::COMPILE, err);
		return 0;
	}
	unsigned int planeLstId[] = { planeVertId, planeFragId };
	ret = planeShader.linkShader(2, planeLstId);
	if (ret != 1)
		return 0;
	CShader quadShader;
	unsigned int quadVertId = quadShader.createShader(CShader::VERTEX);
	ret = quadShader.compileShaderByFile(quadVertId, "./shader//quad.vert.glsl");
	if (ret != 1)
		return 0;
	unsigned int quadFragId = quadShader.createShader(CShader::FRAGMENT);
	ret = quadShader.compileShaderByFile(quadFragId, "./shader//quad.frag.glsl");
	if (ret != 1)
		return 0;
	unsigned int quadLstId[] = { quadVertId, quadFragId };
	ret = quadShader.linkShader(2, quadLstId);
	if (ret != 1)
		return 0;
	while (!glfwWindowShouldClose(window)) {
		if (firstFrame)
			lastFrame = currentFrame;
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		input(window);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.3, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeShader.use();
		cubeTex.useTexture(TEXTURE0);
		glBindVertexArray(cubeVAO);
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		glm::mat4 view = camera.getViewMat();
		glm::mat4 proj = glm::perspective(45.0f, (float)winWidth / winHeight, 1.0f, 100.0f);
		cubeShader.setMatrix4x4("model", model);
		cubeShader.setMatrix4x4("view", view);
		cubeShader.setMatrix4x4("proj", proj);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(cubeVAO);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		cubeShader.setMatrix4x4("model", model);
		cubeShader.setMatrix4x4("view", view);
		cubeShader.setMatrix4x4("proj", proj);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		planeShader.use();
		planeTex.useTexture(TEXTURE0);
		glBindVertexArray(planeVAO);
		model = glm::mat4(1.0);
		planeShader.setMatrix4x4("model", model);
		planeShader.setMatrix4x4("view", view);
		planeShader.setMatrix4x4("proj", proj);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		quadShader.use();
		glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


