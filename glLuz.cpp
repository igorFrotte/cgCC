#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "ObjMesh.h"
#include "SceneNew.h"
#include "Shaders.h"
	
class Skybox{
	ShaderProgram shaderProgram;
	GLTexture cubemap;
	VAO vao;
	GLBuffer vbo;
	int n_verts;

	public:
	void init(std::string files[6]){
		shaderProgram = ShaderProgram{
			Shader{"cubemap.vert", GL_VERTEX_SHADER},
			Shader{"cubemap.frag", GL_FRAGMENT_SHADER}
		};

		cubemap = loadCubemap(files);

		init_mesh("modelos/box.obj");
	}

	void draw(mat4 Projection, mat4 View) const{
		// Apaga translação
		View(0, 3) = View(1,3) = View(2,3) = 0;

		glUseProgram(shaderProgram);
		Uniform{"Projection"} = Projection;
		Uniform{"View"} = View;
		Uniform{"Model"} = loadIdentity();

		glDepthMask(GL_FALSE);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, n_verts);
		glDepthMask(GL_TRUE);
	}

	private:
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

	void init_mesh(std::string filename){
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
};

float angle = 0;

PhongShaderTexNormal shader;
Scene scene;
Light light;
Skybox skybox;
unsigned int screen_width = 600;
unsigned int screen_height = 600;

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);

	shader.init();

	scene.init();

	light.position = vec4{ 0.0, 8.0, 10.0, 1.0 };
	light.Ia = vec3{ 0.2, 0.2, 0.2 };
	light.Id = vec3{ 1.0, 1.0, 1.0 };
	light.Is = vec3{ 1.0, 1.0, 1.0 };
	
	std::string files[6] = {
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};	
	skybox.init(files);
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float a = screen_width/(float)screen_height;
	mat4 Projection = perspective(45, a, 0.1, 50);
	vec4 pos = rotate_y(angle)*vec4{0, 8, 20, 1};
	mat4 View = lookAt(toVec3(pos), {0, 4, 0}, {0, 1, 0});
	
	skybox.draw(Projection, View);

	shader.setup(Projection, View, light);
	scene.draw();
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

