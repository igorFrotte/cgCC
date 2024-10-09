#include "Render2D.h"
#include "matrix.h"
#include "transforms.h"

int main(){
	// Coordenadas do modelo
	std::vector<vec4> V = {
		{-0.5, -0.5, -0.5, 1}, {0.5, -0.5, -0.5, 1}, {0.5, 0.5, -0.5, 1}, 
		{-0.5, 0.5, -0.5, 1},{-0.5, -0.5, 0.5, 1}, {0.5, -0.5, 0.5, 1}, 
		{0.5, 0.5, 0.5, 1}, {-0.5, 0.5, 0.5, 1}, {0, 0.8, -0.5, 1}, {0, 0.8, 0.5, 1}
	};
	unsigned int indices[] = {
		0, 1,	1, 2,	2, 3,	3, 0,	4, 5,	5, 6,
		6, 7,	7, 4,	0, 4,	1, 5,	2, 6,	3, 7,
		9, 7,   9, 6,   3, 8,   8, 2,   9, 8
	};

	int w = 600, h = 600;
	mat4 Screen = {
		w/2.0f,      0,   0, (w-1)/2.0f,
		     0, h/2.0f,   0, (h-1)/2.0f,
		     0,      0,   1,          0,
		     0,      0,   0,          1
	};
	//mat4 Projection = orthogonal(-1.2, 1.2, -1.2, 1.2, 2, 10);
	//mat4 Projection = frustum(-1.2, 1.2, -1.2, 1.2, 2, 10);
	mat4 Projection = perspective(60, w/(float)h, 2, 10);
	mat4 View = lookAt({1, 1, 2}, {0, 0, 0}, {0, 1, 0});
	mat4 Model = loadIdentity();

	mat4 M = Screen*Projection*View*Model;

	Elements<Lines> L{indices};
	
	ImageRGB G(w, h);
	G.fill(white);
	render2d(M*V, L, red, G);
	G.savePNG("output.png");
}
