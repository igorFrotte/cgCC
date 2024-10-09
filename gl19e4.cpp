#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "VertexUtils.h"
#include "transforms.h"
#include "bezier.h"
#include "polygon_triangulation.h"

VAO vao;
GLBuffer vbo;
ShaderProgram shaderProgram;
unsigned int screen_width = 600;
unsigned int screen_height = 600;
unsigned int N;

void init(){
	glewInit();

	shaderProgram = ShaderProgram{
		Shader{"SimpleShader.vert", GL_VERTEX_SHADER},
		Shader{"SimpleShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> Q = sample_bezier_spline<3>(CP,30);

    N = Q.size();
	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(Q, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(vec2),0);
}

void desenha(){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

    float t = (3.14*2)/12;

    for(float i=0; i<12; i++){

        mat3 R = {
            cos(i*t), -sin(i*t), 0.0,
            sin(i*t),  cos(i*t), 0.0,
                0.0,     0.0,    1.0
        };

		mat4 rot = toMat4(R);

		Uniform{"M"} = rot*translate(-0.1, -0.1, 1)*scale(0.5, 0.5, 1.)*orthogonal(0, screen_width, 0, screen_height, -1, 1);
		i == 0 ? Uniform("C") = toVec(red) : Uniform("C") = toVec(blue);

		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_LOOP,0,N);	
	}
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