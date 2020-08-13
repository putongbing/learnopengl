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

//箱子原始位置
float position[] = {
		//顶点				  //法线			  //纹理坐标
		 -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

//点光源位置
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

//十个箱子的位移点
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	unsigned int lvao;
	glGenVertexArrays(1, &lvao);
	glBindVertexArray(lvao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//加载光照贴图
	CTexture2D diffuse;
	bool ret = diffuse.loadTexture("./image/diffuse.png", 0, RGBA, RGBA, UNSIGNED_BYTE);
	if (!ret) {
		std::cout << "Fail to load texture!" << std::endl;
		return 0;
	}
	diffuse.setWrap(WRAP_S, REPEAT);
	diffuse.setWrap(WRAP_T, REPEAT);
	diffuse.setFilter(MIN_FILTER, LINEAR);
	diffuse.setFilter(MAG_FILTER, LINEAR);

	CTexture2D specular;
	ret = specular.loadTexture("./image/diffuse.png", 0, RGBA, RGBA, UNSIGNED_BYTE);
	if (!ret) {
		std::cout << "Fail to load texture!" << std::endl;
		return 0;
	}
	specular.setWrap(WRAP_S, REPEAT);
	specular.setWrap(WRAP_T, REPEAT);
	specular.setFilter(MIN_FILTER, LINEAR);
	specular.setFilter(MAG_FILTER, LINEAR);

	int compileRet = 0;
	int linkRet = 0;
	CShader lightShader;
	unsigned int lightVertex = lightShader.createShader(CShader::VERTEX);
	compileRet = lightShader.compileShaderByFile(lightVertex, "./shader/light.vs");
	if (compileRet != 1) {
		char error[1024];
		lightShader.getCompileOrLinkInfo(lightVertex, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	unsigned int lightFragment = lightShader.createShader(CShader::FRAGMENT);
	compileRet = lightShader.compileShaderByFile(lightFragment, "./shader/light.fs");
	if (compileRet != 1) {
		char error[1024];
		lightShader.getCompileOrLinkInfo(lightFragment, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}

	unsigned int larrID[2] = { lightVertex, lightFragment };
	linkRet = lightShader.linkShader(2, larrID);
	if (linkRet != 1) {
		char error[1024];
		lightShader.getCompileOrLinkInfo(lightShader.getProgramID(), CShader::LINK, error);
		std::cout << error << std::endl;
		return 0;
	}

	CShader shader;
	unsigned int vertex = shader.createShader(CShader::VERTEX);
	compileRet = shader.compileShaderByFile(vertex, "./shader/vertex.vs");
	if (compileRet != 1) {
		char error[1024];
		shader.getCompileOrLinkInfo(vertex, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	unsigned int fragment = shader.createShader(CShader::FRAGMENT);
	compileRet = shader.compileShaderByFile(fragment, "./shader/frag.fs");
	if (compileRet != 1) {
		char error[1024];
		shader.getCompileOrLinkInfo(fragment, CShader::COMPILE, error);
		std::cout << error << std::endl;
		return 0;
	}
	
	unsigned int arrID[2] = { vertex, fragment };
	linkRet = shader.linkShader(2, arrID);
	if (linkRet != 1) {
		char error[1024];
		lightShader.getCompileOrLinkInfo(shader.getProgramID(), CShader::LINK, error);
		std::cout << error << std::endl;
		return 0;
	}
	glEnable(GL_DEPTH_TEST);
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 model(1.0);
	glm::mat4 view(1.0);
	glm::mat4 proj(1.0);
	proj = glm::perspective((float)glm::radians(45.0), (float)winWidth / winHeight, 0.1f, 100.0f);

	shader.use();
	shader.setVec3("dlInfo.dir", glm::vec3(-0.2f, -1.0f, -0.3f));
	shader.setVec3("dlInfo.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.setVec3("dlInfo.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	shader.setVec3("dlInfo.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	shader.setVec3("lightInfo[0].pos", pointLightPositions[0]);
	shader.setVec3("lightInfo[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.setVec3("lightInfo[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("lightInfo[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("lightInfo[1].pos", pointLightPositions[1]);
	shader.setVec3("lightInfo[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.setVec3("lightInfo[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("lightInfo[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("lightInfo[2].pos", pointLightPositions[2]);
	shader.setVec3("lightInfo[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.setVec3("lightInfo[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("lightInfo[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("lightInfo[3].pos", pointLightPositions[3]);
	shader.setVec3("lightInfo[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	shader.setVec3("lightInfo[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("lightInfo[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("slInfo.pos", camera.getPos());
	shader.setVec3("slInfo.slDir", camera.getDir());
	shader.setVec3("slInfo.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("slInfo.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("slInfo.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("slInfo.innerAngle", glm::cos(glm::radians(2.0f)));
	shader.setFloat("slInfo.outAngle", glm::cos(glm::radians(10.0f)));

	shader.setFloat("materialInfo.reflectance", 32.0);
	shader.setInt("materialInfo.diffuse", 0);
	shader.setInt("materialInfo.specular", 1);
	shader.setVec3("viewPos", camera.getPos());
	shader.setFloat("atte.constant", 1.0);
	shader.setFloat("atte.liner", 0.09);
	shader.setFloat("atte.quadratic", 0.032);
	shader.setMatrix4x4("proj", proj);

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
		diffuse.useTexture(0);
		specular.useTexture(1);
		int size = sizeof(cubePositions) / sizeof(cubePositions[0]);
		for (int i = 0; i < size; i++) {
			model = glm::translate(model, cubePositions[i]);
			shader.setMatrix4x4("model", model);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(position) / sizeof(position[0]));
			glBindVertexArray(0);
			model = glm::mat4(1.0);
		}

		lightShader.use();
		lightShader.setMatrix4x4("view", view);
		int lightSize = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
		for (int i = 0; i < lightSize; i++) {
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
			lightShader.setMatrix4x4("model", model);
			glBindVertexArray(lvao);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(position) / sizeof(position[0]));
			glBindVertexArray(0);
			model = glm::mat4(1.0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


