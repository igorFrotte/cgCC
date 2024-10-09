#include "Render2D.h"
#include "bezier.h"
#include "matrix.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	mat2 M = {
		2.2, 0.0,
		0.0, 1.3
	};	

	LineStrip L{P.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	render2d(M*P, L, blue, G);
	G.savePNG("output.png");
}
