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

	// Resolução da imagem.
	int w = 600, h = 600;
	mat4 Screen = {
		w/2.0f,      0,   0, (w-1)/2.0f,
		     0, h/2.0f,   0, (h-1)/2.0f,
		     0,      0,   1,          0,
		     0,      0,   0,          1
	};

	// Volume de visualização
	float l = -1.5, r = 1.5;
	float b = -1.5, t = 1.5;
	float n = 2, f = 10;
	mat4 Volume = {
		2/(r-l),      0,     0,      (l+r)/(l-r),
			0,  2/(t-b),     0,      (b+t)/(b-t),
			0,        0, 2/(f-n),    (n+f)/(n-f),
			0,        0,      0,               1
	};	
	

#define PROJECTION_TYPE 0

#if PROJECTION_TYPE==1 // Projeção ortogonal
	mat4x4 Proj = loadIdentity();

	//mat4 View = loadIdentity(); // projeção no plano xy
	//mat4 View = lookAt({2, 0, 0}, {0, 0, 0}, {0, 1, 0}); // projeção no plano zy
	//mat4 View = lookAt({0, 2, 0}, {0, 0, 0}, {0, 0, 1}); // projeção no plano xz
	//mat4 View = lookAt({1, 1, 1}, {0, 0, 0}, {0, 1, 0}); // isométrica
	//mat4 View = lookAt({1, .2, 1}, {0, 0, 0}, {0, 1, 0}); // dimétrica
	mat4 View = lookAt({.3, .2, 1}, {0, 0, 0}, {0, 1, 0}); // trimétrica
	
////////////////////////////////////////////////////////////
#elif PROJECTION_TYPE==2 // Projeção oblíqua
	mat4 Proj = {
		1, 0, 0.3, 0,
		0, 1, 0.4, 0,
		0, 0, 1.0, 1,
		0, 0, 0.0, 1
	};
	mat4 View = loadIdentity();

////////////////////////////////////////////////////////////
#else // Projeção perspectiva
	// Plano de projeção z = n
	mat4 Proj = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -(n+f)/n, -f,
		0, 0, -1/n, 0
	};
	mat4 View = lookAt({1, 0.7, 2}, {0, 0, 0}, {0, 1, 0});
#endif
	// Matriz do modelo.
	mat4 Model = loadIdentity();

	// Composição das transformações.
	mat4 M = Screen*Volume*Proj*View*Model;

	Elements<Lines> L{indices};
	
	ImageRGB G(w, h);
	G.fill(white);
	render2d(M*V, L, red, G);
	G.savePNG("output.png");
}
