#include "Render2D_v1.h"
#include "bezier.h"

int main(){
	std::vector<vec3> P = {
		{1, 0, 1},
		{1, 1, 1},
		{0, 2, 2},
		//*
		{-1, 1, 1},
		{-1, 0, 1},
		{-1, -1, 1},
		{0, -2, 2},
		{1, -1, 1},
		{1, 0, 1},
		//*/
	};

	mat3 T = {
		140,   0, 150,
		  0, 140, 150,
		  0,   0,   1
	};

	std::vector<vec3> A = T*sample_bezier_spline<2>(P, 50);

	LineStrip L{A.size()};

	ImageRGB G(300, 300);
	G.fill(white);
	render2d(A, L, blue, G);
	G.savePNG("output.png");
}
