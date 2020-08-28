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
	glEnable(GL_DEPTH_TEST);
	CShader shader;
	unsigned int vectexId = shader.createShader(CShader::VERTEX);
	int comRet = shader.compileShaderByFile(vectexId, "./shader//model.vs");
	if (comRet != 1)
		return 0;
	unsigned int fragId = shader.createShader(CShader::FRAGMENT);
	comRet = shader.compileShaderByFile(fragId, "./shader//model.fs");
	if (comRet != 1) {
		char error;
		shader.getCompileOrLinkInfo(fragId, CShader::COMPILE, &error);
		std::cout << error << std::endl;
		return 0;
	}	
	unsigned int lstId[] = { vectexId , fragId };
	comRet = shader.linkShader(2, lstId);
	if (comRet != 1)
		return 0;
	glm::mat4 model(1.0);
	glm::mat4 view(1.0);
	glm::mat4 proj(1.0);
	proj = glm::perspective((float)glm::radians(45.0), (float)winWidth / winHeight, 0.1f, 100.0f);
	shader.use();
	shader.setMatrix4x4("proj", proj);
	shader.setMatrix4x4("model", model);
	shader.setVec3("lightColor", glm::vec3(0.6, 0.6, 0.6));
	shader.setVec3("lightPos", glm::vec3(50, 50, 50));
	glm::vec3 viewPos = camera.getPos();
	shader.setVec3("viewPos", viewPos);
	CModelX m;
	m.loadModel("./nanosuit//nanosuit.obj");
	while (!glfwWindowShouldClose(window)) {
		if (firstFrame)
			lastFrame = currentFrame;
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		input(window);
		glClearColor(0.2, 0.3, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view = camera.getViewMat();
		shader.use();
		shader.setMatrix4x4("view", view);
		m.draw(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


