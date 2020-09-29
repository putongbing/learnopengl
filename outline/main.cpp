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
	// positions        // texture Coords 
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	int size = 5 * sizeof(float);

	//cube
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(3 * sizeof(float)));

	//plane
	unsigned int vaoPlane;
	glGenVertexArrays(1, &vaoPlane);
	glBindVertexArray(vaoPlane);

	unsigned int vboPlane;
	glGenBuffers(1, &vboPlane);
	glBindBuffer(GL_ARRAY_BUFFER, vboPlane);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	CTexture2D tex;
	bool ret = tex.loadTexture("./image//wall.jpg", 0, UNSIGNED_BYTE);
	if (!ret) {
		std::cout << "Failed to load texture!" << std::endl;
		return 0;
	}

	CShader shader;
	unsigned int vertexId = shader.createShader(CShader::VERTEX);
	ret = shader.compileShaderByFile(vertexId, "./shader//outline.vs");
	if (ret != 1) {
		char error[1024];
		shader.getCompileOrLinkInfo(vertexId, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	unsigned int fragId = shader.createShader(CShader::FRAGMENT);
	ret = shader.compileShaderByFile(fragId, "./shader//outline.fs");
	if (ret != 1) {
		char error[1024];
		shader.getCompileOrLinkInfo(fragId, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	unsigned int lstId[] = { vertexId , fragId };
	ret = shader.linkShader(2, lstId);
	if (ret != 1) {
		char error[1024];
		shader.getCompileOrLinkInfo(shader.getProgramID(), CShader::LINK, error);
		std::cout << error << std::endl;
		return 0;
	}

	CShader outline;
	unsigned int olVertexId = outline.createShader(CShader::VERTEX);
	ret = outline.compileShaderByFile(olVertexId, "./shader//outline_l.vs");
	if (ret != 1) {
		char error[1024];
		outline.getCompileOrLinkInfo(olVertexId, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	unsigned int loFragId = outline.createShader(CShader::FRAGMENT);
	ret = outline.compileShaderByFile(loFragId, "./shader//outline_l.fs");
	if (ret != 1) {
		char error[1024];
		outline.getCompileOrLinkInfo(loFragId, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	unsigned int olLstId[] = { olVertexId , loFragId };
	ret = outline.linkShader(2, olLstId);
	if (ret != 1) {
		char error[1024];
		outline.getCompileOrLinkInfo(outline.getProgramID(), CShader::LINK, error);
		std::cout << error << std::endl;
		return 0;
	}

	shader.use();
	shader.setInt("tex", 0);
	glm::mat4 view(1.0);
	glm::mat4 proj(1.0);
	proj = glm::perspective((float)glm::radians(45.0), (float)winWidth / (float)winHeight, 1.0f, 100.0f);
	shader.setMatrix4x4("proj", proj);
	outline.use();
	outline.setMatrix4x4("proj", proj);
	while (!glfwWindowShouldClose(window)) {
		if (firstFrame)
			lastFrame = currentFrame;
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glClearColor(0.2, 0.3, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		input(window);
		//绘制地板
		glm::mat4 model(1.0);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0);
		shader.use();
		shader.setMatrix4x4("model", model);
		glBindVertexArray(vaoPlane);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//绘制箱子
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);;
		shader.setMatrix4x4("view", view);
		model = glm::mat4(1.0);
		shader.setMatrix4x4("model", model);
		view = camera.getViewMat();
		tex.useTexture(TEXTURE0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMatrix4x4("model", model);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//绘制轮廓
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0);
		glDisable(GL_DEPTH_TEST);
		outline.use();
		model = glm::mat4(1.0);
		float s = 1.1;
		model = glm::scale(model, glm::vec3(s));
		outline.setMatrix4x4("model", model);
		view = camera.getViewMat();
		outline.setMatrix4x4("view", view);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(s));
		outline.setMatrix4x4("model", model);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


