#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "VertexUtils.h"
#include "MixColorShader.h"
#include "MarchingCubes.h"
#include "transforms.h"

ShaderProgram shaderProgram;
VAO vao;
GLBuffer vbo;
int n_verts;
float angle = 0;
unsigned int screen_width = 600;
unsigned int screen_height = 600;
	
float sphere(float x, float y, float z){
	float r = 1.5;
	return x*x + y*y + z*z - r*r;
}

float torus(float x, float y, float z){
	float R = 1.5;
	float r = 0.4;
	return pow(sqrt(x*x + y*y) - R, 2) + z*z - r*r;
}

float treco(float x, float y, float z){
	float r = (x*x + y*y);
	return 2*y*(y*y - 3*x*x)*(1-z*z) + r*r - (9*z*z - 1)*(1 - z*z);
}

void init_surface(int m, int n, int p){
	vec3 pmin = {-2, -2, -2}; 
	vec3 pmax = { 2,  2,  2};

	Uniform{"pmin"} = pmin;
	Uniform{"pmax"} = pmax;
	Uniform{"C"} = {
		toVec(red),    toVec(blue),    toVec(green), toVec(yellow), 
		toVec(orange), toVec(magenta), toVec(cyan),  toVec(purple)
	};
	
	std::vector<vec3> V = marchingCubes(treco, m, n, p, pmin, pmax);

	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(V);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	n_verts = V.size();
}

void init(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	shaderProgram = ShaderProgram{
		Shader{"MixColorShader.vert", GL_VERTEX_SHADER},
		Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);

	init_surface(50, 50, 50);
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 Model = rotate_z(angle);
	mat4 View = lookAt({3, 3, 1.5}, {0, 0, 0}, {0, 0, 1});
	float a = screen_width/(float)screen_height;
	mat4 Projection = perspective(50, a, 0.1, 50);

	Uniform{"M"} = Projection*View*Model;

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, n_verts);
}

double last_x;

void mouse_button_callback(GLFWwindow* window, int button, int action, int /*mods*/){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		last_x = x;
	}
}

void cursor_position_callback(GLFWwindow* window, double x, double /*y*/){
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){
		double dx = x - last_x;
		angle += dx*0.01;
		last_x = x;
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

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "CG UFF", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	

	glewInit();
	init();

	glViewport(0, 0, screen_width, screen_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	
	std::cout << "GL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	while(!glfwWindowShouldClose(window)){
		desenha();
    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}
	glfwTerminate();
}

