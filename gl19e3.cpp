#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "VertexUtils.h"
#include "transforms.h"
#include "bezier.h"
#include "polygon_triangulation.h"

VAO vao1, vao2;
GLBuffer vbo1, vbo2;
ShaderProgram shaderProgram;
unsigned int screen_width = 800;
unsigned int screen_height = 800;

vec2 P[] = {
    {11, 218},  {184, 10},  {54, 344},
    {315, 369}, {228, 281}, {349, 180}
};

std::vector<vec2> Q = sample_bezier<5>(P,100);

void init(){
	glewInit();

	shaderProgram = ShaderProgram{
		Shader{"SimpleShader.vert", GL_VERTEX_SHADER},
		Shader{"SimpleShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

	vao1 = VAO{true};
	glBindVertexArray(vao1);

	vbo1 = GLBuffer{GL_ARRAY_BUFFER};
	vbo1.data(P, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(vec2),0);

	vao2 = VAO{true};
	glBindVertexArray(vao2);

	vbo2 = GLBuffer{GL_ARRAY_BUFFER};
	vbo2.data(Q, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(vec2),0);
}

void desenha(){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	Uniform{"M"} = translate(0.7, 0.7, 0)*scale(1.3, 1.3, 1.)*orthogonal(0, screen_width, 0, screen_height, -1, 1);

	glBindVertexArray(vao1);
    Uniform{"C"} = toVec(red);
    glDrawArrays(GL_LINE_STRIP, 0, 6);

	glBindVertexArray(vao2);
    Uniform{"C"} = toVec(blue);
    glDrawArrays(GL_LINE_STRIP, 0, Q.size());	
}

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height){
    glViewport(0, 0, width, height);
	screen_width = width;
	screen_height = height;
}

int main(){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "OpenGL", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	init();

	glViewport(0, 0, screen_width, screen_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::cout << "GL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	while(!glfwWindowShouldClose(window)){
		desenha();
    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}
	glfwTerminate();
}