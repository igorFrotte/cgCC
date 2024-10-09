#include "Render2D.h"
#include "matrix.h"
#include "bezier.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	float t = 0.6;
	mat2 M = {
		cos(t), -sin(t),
		sin(t), cos(t)
	};

	LineStrip L{P.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	render2d(M*P, L, blue, G);
	G.savePNG("output.png");
}
