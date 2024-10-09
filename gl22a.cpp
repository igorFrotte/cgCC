#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "transforms.h"

// Controle da rotacao
mat4 R = loadIdentity();

// Variaveis do shader
ShaderProgram shaderProgram;
ShaderProgram backface_shaderProgram;
ShaderProgram quad_shaderProgram;

unsigned int screen_width = 600;
unsigned int screen_height = 600;
	
// Framebuffer e textura para guardar backface 
unsigned int backface_FBO;
int BACKFACE_WIDTH = 1024, BACKFACE_HEIGHT = 1024;
GLTexture backface_texture;

// Vertex Array Object do Cubo
VAO cube_vao;
GLBuffer cube_vbo;
GLBuffer cube_ebo;

// Dados volumetricos
GLTexture volume_texture;

// Dados da funcao de transferencia
GLTexture tf_texture;

// Vertex Array Object do quad
VAO quad_vao;
GLBuffer quad_vbo;
GLBuffer quad_ebo;

float vmin = 0.5;
float vmax = 0.7;
float stepSize = 0.001f;

////////////////////////////////////////////////////////////////////////////////
// FUNCOES DE INICIALIZACAO

void initCube(){
	// Vertices 
	vec3 P[] = {
		{0, 0, 0},	{1, 0, 0},	{1, 1, 0},	{0, 1, 0},
		{0, 0, 1},	{1, 0, 1},	{1, 1, 1},	{0, 1, 1},
	};
	unsigned int indices[] = {
		0, 2, 1,	0, 3, 2, // back
		4, 5, 7,	5, 6, 7, // front
		5, 1, 6,	1, 2, 6, // right
		0, 4, 3,	4, 7, 3, // left
		0, 1, 4,	1, 5, 4, // down
		2, 3, 6,	3, 7, 6  // up
	};
	
	cube_vao = VAO{true};
	glBindVertexArray(cube_vao);

	cube_vbo = GLBuffer{GL_ARRAY_BUFFER};

	cube_vbo.data(P);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	cube_ebo = GLBuffer{GL_ELEMENT_ARRAY_BUFFER};
	cube_ebo.data(indices);
}

void initBackfaceBuffer(){
	backface_texture = GLTexture{GL_TEXTURE_2D};
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 
		BACKFACE_WIDTH, BACKFACE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 
		BACKFACE_WIDTH, BACKFACE_HEIGHT);

	glGenFramebuffers(1, &backface_FBO);  
	glBindFramebuffer(GL_FRAMEBUFFER, backface_FBO);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, backface_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initVolume(){
	size_t w = 256, h = 256, d = 225;
	size_t size = w*h*d;
	std::vector<char> volume(size); 

	std::ifstream datafile("volume_data/head256.raw");
	if(!datafile){
		std::cerr << "could not open file\n";
		exit(1);
	}
	datafile.read(volume.data(), size);
	if(!datafile){
		std::cerr << "could not read file\n";
		exit(1);
	}
	
	volume_texture = GLTexture{GL_TEXTURE_3D};
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, w, h, d, 0, GL_RED, GL_UNSIGNED_BYTE, volume.data());
}

void initShaders(){
	shaderProgram = ShaderProgram{
		Shader{"volume_data/raycasting.vert", GL_VERTEX_SHADER}, 
		Shader{"volume_data/raycasting.frag", GL_FRAGMENT_SHADER}
	};

	backface_shaderProgram = ShaderProgram{
		Shader{"volume_data/backface.vert", GL_VERTEX_SHADER}, 
		Shader{"volume_data/backface.frag", GL_FRAGMENT_SHADER}
	};
}

void init(){
	glewInit();
	enable_debug();
	
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	initShaders();

	initCube();
	
	initBackfaceBuffer();

	initVolume();
}

////////////////////////////////////////////////////////////////////////////////
// FUNCOES DE DESENHO

void renderScene(GLenum cull_face){
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float aspect = screen_width/(float)screen_height;
	mat4 Projection = scale(1, 1, -1)*perspective(60, aspect, .1, 500);
	mat4 View = translate(0, 0, -1.5);
	mat4 Model = R*translate(-0.5, -0.5, -0.5);
	Uniform{"MVP"} = Projection*View*Model;

	glCullFace(cull_face);
	glBindVertexArray(cube_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawBackface(){
	glBindFramebuffer(GL_FRAMEBUFFER, backface_FBO);
	glViewport(0, 0, BACKFACE_WIDTH, BACKFACE_HEIGHT);

	glUseProgram(backface_shaderProgram);
	renderScene(GL_FRONT);
}

void setup_uniforms(int w, int h){
	Uniform{"ScreenSize"} = vec2{(float)w, (float)h};
	Uniform{"StepSize"} = stepSize;

	Uniform{"vmin"} = vmin;
	Uniform{"vmax"} = vmax;

	Uniform{"ExitPoints"} = 0;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backface_texture);

	Uniform{"VolumeTex"} = 1;
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, volume_texture);
	
	Uniform{"TransferFunction"} = 2;
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, tf_texture);
}

void drawScene(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width, screen_height);

	glUseProgram(shaderProgram);
	setup_uniforms(screen_width, screen_height);

	renderScene(GL_BACK);
}

void desenha(){
	drawBackface();
	drawScene();
}


////////////////////////////////////////////////////////////////////////////////
// FUNCOES CALLBACK
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

void special_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/){
	if(action != GLFW_PRESS && action != GLFW_REPEAT)
		return;

	if(key == GLFW_KEY_DOWN)
		vmin = fmax(0, vmin - 0.01);

	if(key == GLFW_KEY_UP)
		vmin = fmin(vmax - 0.01, vmin + 0.01);
	
	if(key == GLFW_KEY_LEFT)
		vmax = fmax(vmin + 0.01, vmax - 0.01);

	if(key == GLFW_KEY_RIGHT)
		vmax = fmin(1, vmax + 0.01);

	std::cout << "range: [" << vmin << ", " << vmax << "]\n"; 
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

