// window.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"
#include "common.h"
#include "CTexture2D.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float position[] = {
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//生成纹理单元1
	CTexture2D tex0;
	//重复环绕方式
	tex0.setFilter(WRAP_S, REPEAT);
	tex0.setFilter(WRAP_T, REPEAT);
	//线性过滤方式
	tex0.setWrap(MIN_FILTER, LINEAR);
	tex0.setWrap(MAG_FILTER, LINEAR);

	//加载纹理
	tex0.loadTexture("./wall.jpg", 0, RGB, RGB, UNSIGNED_BYTE);

	//生成纹理单元1
	CTexture2D tex1;
	//重复环绕方式
	tex1.setFilter(WRAP_S, REPEAT);
	tex1.setFilter(WRAP_T, REPEAT);
	//线性过滤方式
	tex1.setWrap(MIN_FILTER, LINEAR);
	tex1.setWrap(MAG_FILTER, LINEAR);

	//加载纹理
	tex1.loadTexture("./smile.png", 0, RGBA, RGBA, UNSIGNED_BYTE);

	CShader shader;
	int compileRet = 0;
	unsigned int vertex = shader.createShader(CShader::VERTEX);
	//compileRet = shader.compileShader(vertex, vsCode);
	compileRet = shader.compileShaderByFile(vertex, "./shader/vertex.vs");
	if (compileRet == -1) {
		std::cout << shader.getCompileOrLinkInfo(vertex, CShader::COMPILE) << std::endl;
		return 0;
	}
	unsigned int fragment = shader.createShader(CShader::FRAGMENT);
	//shader.compileShader(fragment, fsCode);
	compileRet = shader.compileShaderByFile(fragment, "./shader/frag.fs");
	if (compileRet == -1) {
		std::cout << shader.getCompileOrLinkInfo(fragment, CShader::COMPILE) << std::endl;
		return 0;
	}
	int linkRet = 0;
	unsigned int arrID[2] = { vertex, fragment };
	linkRet = shader.linkShader(2, arrID);
	if (linkRet == -1) {
		std::cout << shader.getCompileOrLinkInfo(fragment, CShader::LINK) << std::endl;
		return 0;
	}

	shader.use();
	shader.setInt("wallTex", 0);
	shader.setInt("smileTex", 1);
	//glm::mat4 matProj = glm::perspective(glm::radians(45), (float)winWidth / winHeight, 0.0, 1.0);
	//shader.setMatrix4x4("proj", );
	//glm::mat4 matView = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//shader.setMatrix4x4("view", );

	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0.2, 0.3, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		/*绘制三角形*/
		tex0.useTexture(TEXTURE0);
		tex1.useTexture(TEXTURE1);
		shader.use();
		glBindVertexArray(vao);
		glm::mat4 model(1.0);
		model = glm::rotate(model, glm::radians(glfwGetTime()), glm::vec3(1.0, 0.0, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

