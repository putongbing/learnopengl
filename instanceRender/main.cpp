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
#include "CMesh.h"
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


glm::mat4* generateModelMat(int amount) {
	float radius = 150.0f;
	float offset = 25.0f;
	srand(glfwGetTime());
	glm::mat4* lstModel = new glm::mat4[amount];
	for (int i = 0; i < amount; i++) {
		glm::mat4 model(1.0);
		// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; 
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. 缩放：在 0.05 和 0.25f 之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. 添加到矩阵的数组中
		lstModel[i] = model;
	}
	return lstModel;
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

	CShader shader;
	unsigned int vertexId = shader.createShader(CShader::VERTEX);
	int ret = shader.compileShaderByFile(vertexId, "./shader//instance.vert.glsl");
	if (ret != 1) {
		char err[1024] = "";
		shader.getCompileOrLinkInfo(vertexId, CShader::COMPILE, err);
		return 0;
	}
	unsigned int fragId = shader.createShader(CShader::FRAGMENT);
	ret = shader.compileShaderByFile(fragId, "./shader//instance.frag.glsl");
	if (ret != 1) {
		char err[1024] = "";
		shader.getCompileOrLinkInfo(fragId, CShader::COMPILE, err);
		return 0;
	}
	unsigned int lstId[] = { vertexId, fragId };
	ret = shader.linkShader(2, lstId);
	if (ret != 1)
		return 0;

	CModelX m;
	m.loadModel("./planet//planet.obj");
	int amount = 10000;
	glm::mat4* modelMat = generateModelMat(amount);
	//处理实例化数组
	unsigned int instance;
	glGenBuffers(1, &instance);
	glBindBuffer(GL_ARRAY_BUFFER, instance);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMat[0], GL_STATIC_DRAW);
	std::vector<CMesh> meshes = m.getMeshes();
	for (int i = 0; i < meshes.size(); i++) {
		CMesh mesh = meshes[i];
		unsigned int vao = mesh.getVAO();
		glBindVertexArray(vao);
		int size = sizeof(glm::mat4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, size, 0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, size, (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);					
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, size, (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);					
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, size, (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}

	glm::mat4 view(1.0);
	glm::mat4 proj = glm::perspective(40.0, (double)winWidth / winHeight, 1.0, 1000.0);
	shader.use();
	shader.setMatrix4x4("proj", proj);
	glEnable(GL_DEPTH_TEST);
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
		for (int i = 0; i < meshes.size(); i++) {
			CMesh mesh = meshes[i];
			glBindVertexArray(mesh.getVAO());
			glActiveTexture(GL_TEXTURE0);
			CTexture2D tex = mesh.getTexture()[0].m_tex;
			tex.useTexture(TEXTURE0);
			std::vector<unsigned int> vecElement = mesh.getElement();
			glDrawElementsInstanced(GL_TRIANGLES, vecElement.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


