#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "ObjMesh.h"
#include "transforms.h"

VAO vao;
GLBuffer vbo;
int n_verts;
GLTexture texture;
ShaderProgram shaderProgram;
mat4 Model;
mat4 R;
unsigned int screen_width = 600;
unsigned int screen_height = 600;

GLTexture loadCubemap(std::string files[6]){
	GLTexture cubemap{GL_TEXTURE_CUBE_MAP};

	cubemap.load(files[0], GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	cubemap.load(files[1], GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	cubemap.load(files[2], GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	cubemap.load(files[3], GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	cubemap.load(files[4], GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	cubemap.load(files[5], GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return cubemap;
}

void init_surface(std::string filename){
	ObjMesh mesh{filename};
	auto mesh_triangles = mesh.getTriangles();
	
	vao = VAO{true};
	glBindVertexArray(vao);

	vbo = GLBuffer{GL_ARRAY_BUFFER};
	vbo.data(mesh_triangles);

	size_t stride = sizeof(ObjMesh::Vertex);
	size_t offset_position = offsetof(ObjMesh::Vertex, position);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
	
	n_verts = mesh_triangles.size();
}

void init(){
	glewInit();
	enable_debug();

	glEnable(GL_DEPTH_TEST);

	shaderProgram = ShaderProgram{
		Shader{"cubemap.vert", GL_VERTEX_SHADER},
		Shader{"cubemap.frag", GL_FRAGMENT_SHADER}
	};
	glUseProgram(shaderProgram);
	
	std::string files[6] = {
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};	
	texture = loadCubemap(files);

	//init_surface("modelos/box.obj");
	//Model = scale(2, 2, 2);
	
	init_surface("modelos/bunny.obj");	
	Model = scale(2, 2, 2)*translate(0, -0.5, 0);

	R = loadIdentity();
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float aspect = screen_width/(float)screen_height;
	Uniform{"Projection"} = perspective(50, aspect, 1, 50);
	Uniform{"View"} = lookAt({0, 0, 10}, {0, 0, 0}, {0, 1, 0})*R;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	Uniform{"Model"} = Model;
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, n_verts);
}

double last_x;
double last_y;

void mouse_button_callback(GLFWwindow* window, int button, int action, int /*mods*/){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		last_x = x;
		last_y = y;
	}
}

void cursor_position_callback(GLFWwindow* window, double x, double y){
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){
		double dx = x - last_x;
		double dy = y - last_y;
		R = rotate_y(dx*0.01)*rotate_x(dy*0.01)*R;
		last_x = x;
		last_y = y;
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


