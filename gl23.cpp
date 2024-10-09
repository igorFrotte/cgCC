#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "utilsGL.h"
#include "transforms.h"

VAO vao;
ShaderProgram shaderProgram;
unsigned int screen_width = 600;
unsigned int screen_height = 600;

// Controle da rotacao
mat4 R = loadIdentity();
mat4 BaseView = lookAt({0, 0, 5}, {0, 0, 0}, {0, 1, 0});
float vangle = 0;

int shadow_type = 0;

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST); 

	enable_debug();
	
	shaderProgram = ShaderProgram{
		Shader{"RayTracing.vert", GL_VERTEX_SHADER},
		Shader{"RayTracing.frag", GL_FRAGMENT_SHADER}
	};

	vao = VAO{true};
}

void desenha(){
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float aspect = (float)screen_width/(float)screen_height;

	glUseProgram(shaderProgram);
	Uniform{"Projection"} = scale(1, 1, -1)*perspective(70, aspect, .5, 50);
	Uniform{"View"} = rotate_x(vangle)*BaseView;
	Uniform{"ShadowType"} = shadow_type;

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

double last_x, last_y;

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

		vangle += 0.01*dy;
		vangle = clamp(vangle, -1.5, 1.5);
		BaseView = rotate_y(dx*0.01)*BaseView;

		last_x = x;
		last_y = y;	
	}
}
void special_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/){
	if(action != GLFW_PRESS && action != GLFW_REPEAT)
		return;
	
	vec3 dir = {0, 0, 0};
	float zmove = 0;
	float xmove = 0;

	if(key == GLFW_KEY_UP)
		zmove += 0.2;

	if(key == GLFW_KEY_DOWN)
		zmove -= 0.2;

	if(key == GLFW_KEY_LEFT)
		xmove += 0.2;

	if(key == GLFW_KEY_RIGHT)
		xmove -= 0.2;

	if(key == GLFW_KEY_0 || key == GLFW_KEY_3)
		shadow_type = 0;
	if(key == GLFW_KEY_1)
		shadow_type = 1;
	if(key == GLFW_KEY_2)
		shadow_type = 2;


	BaseView = translate(xmove, 0, zmove)*BaseView;
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
	glfwSetKeyCallback(window, special_callback);
	
	std::cout << "GL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	while(!glfwWindowShouldClose(window)){
		desenha();
    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}
	glfwTerminate();
}
