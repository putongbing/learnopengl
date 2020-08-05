// window.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"

void windowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float position[] = {
	0.5f, 0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,// 右上角
	0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0, // 右下角
	-0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0,// 左下角
	-0.5f, 0.5f, 0.0f, 0.5, 0.5, 0.0, 1.0  // 左上角
};

unsigned int index[] = {
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

const char* vsCode = {
	"#version 330 core									\n"
	"layout (location = 0) in vec3 pos;					\n"
	"void main(){										\n"
	"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);	\n"
	"}													\n"
};

const char* fsCode = {
	"#version 330 core							\n"
	"out vec4 fragColor;						\n"
	"void main() {								\n"
	"	fragColor = vec4(1.0, 0.0, 0.0, 1.0);	\n"
	"}											\n"
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

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0.2, 0.3, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		/*绘制三角形*/
		shader.use();
		std::vector<float> color = { 1.0, 0.0, 0.0, 1.0 };
		shader.setVec4("ufColor", color);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


