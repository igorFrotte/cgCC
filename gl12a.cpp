#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "ObjMesh.h"
#include "transforms.h"

VAO vao;
GLBuffer vbo;
ShaderProgram shaderProgram;
int n_verts;
float angle = 0;
unsigned int screen_width = 600;
unsigned int screen_height = 600;

void init_shader(){
	shaderProgram = ShaderProgram{
		//Shader{"PhongShader.vert", GL_VERTEX_SHADER},
		//Shader{"PhongShader.frag", GL_FRAGMENT_SHADER}'
		Shader{"GouraudShader.vert", GL_VERTEX_SHADER},
		Shader{"ColorShader.frag", GL_FRAGMENT_SHADER}
	};
}

void init_surface(){
	ObjMesh mesh{"modelos/teapot.obj"};
	auto mesh_triangles = mesh.getTriangles();
	
	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(mesh_triangles);

	size_t stride = sizeof(ObjMesh::Vertex);
	size_t offset_position = offsetof(ObjMesh::Vertex, position);
	size_t offset_normal = offsetof(ObjMesh::Vertex, normal);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,(void*)offset_normal);

	n_verts = mesh_triangles.size();
}

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	init_shader();
	init_surface();
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 Model = rotate_z(angle);
	mat4 View = lookAt({23, 15, 23}, {0, 0, 8}, {0, 0, 1});
	float a = screen_width/(float)screen_height;
	mat4 Projection = perspective(45, a, 0.1, 50);

	glUseProgram(shaderProgram);
	Uniform{"Projection"} = Projection;
	Uniform{"View"} = View;
	Uniform{"Model"} = Model;
	
	Uniform{"light_position"} = vec4{ 0, 15, 13, 1.0};
	Uniform{"Ia"} = vec3{ 0.2, 0.2, 0.2};
	Uniform{"Id"} = vec3{ 1.0, 1.0, 1.0};
	Uniform{"Is"} = vec3{ 1.0, 1.0, 1.0};
		
	Uniform{"Ka"} = vec3{1, 1, 1};
	Uniform{"Kd"} = vec3{0.5, 0.5, 0.5};
	Uniform{"Ks"} = vec3{0.5, 0.5, 0.5};
	Uniform{"shininess"} = 100.0f;

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

