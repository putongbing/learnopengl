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

float position[] = {
	/*顶点				，法线*/	
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

int main()
{
	glfwInit();
	//use opengl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

	/*设置vao，vbo*/
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	unsigned int lvao;
	glGenVertexArrays(1, &lvao);
	glBindVertexArray(lvao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	int compileRet = 0;
	int linkRet = 0;
	CShader lightShader;
	unsigned int lightVertex = lightShader.createShader(CShader::VERTEX);
	compileRet = lightShader.compileShaderByFile(lightVertex, "./shader/light.vs");
	if (compileRet != 1) {
		std::cout << lightShader.getCompileOrLinkInfo(lightVertex, CShader::COMPILE) << std::endl;
		return 0;
	}
	unsigned int lightFragment = lightShader.createShader(CShader::FRAGMENT);
	compileRet = lightShader.compileShaderByFile(lightFragment, "./shader/light.fs");
	if (compileRet != 1) {
		std::cout << lightShader.getCompileOrLinkInfo(lightFragment, CShader::COMPILE) << std::endl;
		return 0;
	}

	unsigned int larrID[2] = { lightVertex, lightFragment };
	linkRet = lightShader.linkShader(2, larrID);
	if (linkRet != 1) {
		std::cout << lightShader.getCompileOrLinkInfo(lightFragment, CShader::LINK) << std::endl;
		return 0;
	}

	CShader shader;
	unsigned int vertex = shader.createShader(CShader::VERTEX);
	compileRet = shader.compileShaderByFile(vertex, "./shader/vertex.vs");
	if (compileRet != 1) {
		std::cout << shader.getCompileOrLinkInfo(vertex, CShader::COMPILE) << std::endl;
		return 0;
	}
	unsigned int fragment = shader.createShader(CShader::FRAGMENT);
	compileRet = shader.compileShaderByFile(fragment, "./shader/frag.fs");
	if (compileRet != 1) {
		std::cout << shader.getCompileOrLinkInfo(fragment, CShader::COMPILE) << std::endl;
		return 0;
	}
	
	unsigned int arrID[2] = { vertex, fragment };
	linkRet = shader.linkShader(2, arrID);
	if (linkRet != 1) {
		std::cout << shader.getCompileOrLinkInfo(fragment, CShader::LINK) << std::endl;
		return 0;
	}
	glEnable(GL_DEPTH_TEST);
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 model(1.0);
	glm::mat4 view(1.0);
	glm::mat4 proj(1.0);
	proj = glm::perspective((float)glm::radians(45.0), (float)winWidth / winHeight, 0.1f, 100.0f);

	shader.use();
	shader.setVec3("lightInfo.pos", lightPos);
	shader.setVec3("lightInfo.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setVec3("lightInfo.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3("lightInfo.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("materialInfo.reflectance", 32.0);
	shader.setVec3("materialInfo.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	shader.setVec3("materialInfo.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	shader.setVec3("materialInfo.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3("viewPos", camera.getPos());
	shader.setMatrix4x4("proj", proj);
	shader.setMatrix4x4("model", model);

	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	lightShader.use();
	lightShader.setMatrix4x4("proj", proj);
	lightShader.setMatrix4x4("model", model);
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
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(position) / sizeof(position[0]));
		glBindVertexArray(0);
		lightShader.use();
		lightShader.setMatrix4x4("view", view);
		glBindVertexArray(lvao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(position) / sizeof(position[0]));
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


