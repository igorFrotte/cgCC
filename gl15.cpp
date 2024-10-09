#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "transforms.h"

GLTexture init_texture(std::string image, GLenum active_texture){
	glActiveTexture(active_texture);

	GLTexture texture{GL_TEXTURE_2D};
	texture.load(image);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(GLEW_EXT_texture_filter_anisotropic){
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}
	return texture;
}

struct AppData{
	ShaderProgram shaderProgram = {
			Shader{"PhongShaderTex.vert", GL_VERTEX_SHADER},
			Shader{"PhongShaderTex.frag", GL_FRAGMENT_SHADER}
		};

	VAO vao;
	GLBuffer vbo;

	GLTexture diffuse_map = init_texture("brickwall.jpg", GL_TEXTURE0);
	GLTexture normal_map  = init_texture("brickwall_normal.jpg", GL_TEXTURE2);

	mat4 Model = loadIdentity();

	AppData(){
		enable_debug();
		glEnable(GL_DEPTH_TEST);
		
		struct Vertex{
			vec2 position;
			vec2 texCoords;
			vec3 normal;
		};

		Vertex V[] = {
			{{-5, -5}, {0, 0}, {0, 0, 1}},
			{{ 5, -5}, {1, 0}, {0, 0, 1}},
			{{ 5,  5}, {1, 1}, {0, 0, 1}},
			{{-5,  5}, {0, 1}, {0, 0, 1}}
		};
		
		vao = VAO{true};
		glBindVertexArray(vao);

		vbo = GLBuffer{GL_ARRAY_BUFFER};
		vbo.data(V);
		
		size_t stride = sizeof(Vertex);
		size_t offset_position = offsetof(Vertex, position);
		size_t offset_texCoords = offsetof(Vertex, texCoords);
		size_t offset_normal = offsetof(Vertex, normal);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset_position);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset_texCoords);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset_normal);
	}

	void draw(int screen_width, int screen_height){
		glClearColor(1, 1, 1, 1);	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 View = lookAt({0, 0, 15}, {0, 0, 0}, {0, 1, 0});
		float a = screen_width/(float)screen_height;
		mat4 Projection = perspective(50, a, 0.1, 50);

		glUseProgram(shaderProgram);
		
		Uniform{"Ka"} = vec3{ 1, 1, 1};
		Uniform{"Kd"} = vec3{ 1, 1, 1};
		Uniform{"Ks"} = vec3{ 0.3, 0.3, 0.3};
		Uniform{"shininess"} = 40.0f;

		Uniform{"light_position"} = vec4{ 0.0, 0.0, 10.0, 1.0 };
		Uniform{"Ia"} =  vec3{ 0.0, 0.0, 0.0};
		Uniform{"Id"} =  vec3{ 0.8, 0.8, 0.8};
		Uniform{"Is"} = vec3{ 1.0, 1.0, 1.0};

		Uniform{"map_Kd"} = 0;
		Uniform{"has_map_Kd"} = true;
		
		Uniform{"normal_map"} = 2;
		Uniform{"has_normal_map"} = true;

		Uniform{"Projection"} = Projection;
		Uniform{"View"} = View;
		Uniform{"Model"} = Model;

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
};

static double last_x;
static double last_y;

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
		AppData* data = (AppData*)glfwGetWindowUserPointer(window);
		data->Model = rotate_y(dx*0.01)*rotate_x(dy*0.01)*data->Model;
		last_x = x;
		last_y = y;
	}
}

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height){
    glViewport(0, 0, width, height);
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

	int screen_width = 800;
	int screen_height = 600;
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "CG UFF", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	
		
	glewInit();

	AppData data;
	glfwSetWindowUserPointer(window, (void*)&data);

	glViewport(0, 0, screen_width, screen_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	
	std::cout << "GL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	while(!glfwWindowShouldClose(window)){
		glfwGetWindowSize(window, &screen_width, &screen_height);
		data.draw(screen_width, screen_height);
    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}
	glfwTerminate();
}

