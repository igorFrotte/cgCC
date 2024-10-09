#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const char* source_vs =
	"#version 330\n                  "
	"layout (location=0) in vec4 v;  "
	"void main(){                    "
	"    gl_Position = v;            "
	"}                               ";

const char* source_fs =
	"#version 330\n                        "
	"out vec4 fragColor;                   "
	"void main(){ 	                       "
	"    fragColor = vec4(0, 0, 1, 1);     "
	"}                                     ";

unsigned int shaderProgram;
unsigned int vao;

void init_shaders(){
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source_vs, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &source_fs, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
}

void init_buffers(){
	struct vec2{ 
		float x, y; 
	};

	vec2 V[] = {
		{-0.8, -0.8},
		{-0.4, -0.6},
		{ 0.4, -0.8},
		{ 0.8, -0.4},
		{-0.6,  0.6},
		{ 0.6,  0.0},
	};

	unsigned int indices[] = {0, 4, 5, 3, 2, 1};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo_position;
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(V), V, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void init(){

	init_shaders();
	
	init_buffers();
}

void desenha(){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	GLFWwindow* window = glfwCreateWindow(400, 400, "Janela", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);	

	glewInit();
	init();
	std::cout << "GL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	glViewport(0, 0, 400, 400);
	
	while(!glfwWindowShouldClose(window)){
		desenha();
    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}
	glfwTerminate();
}
