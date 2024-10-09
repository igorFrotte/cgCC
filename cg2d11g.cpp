#include "Render2D.h"
#include "bezier.h"
#include "matrix.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	float t = 0.5;
	mat2 Rot = {
		cos(t), -sin(t),
		sin(t), cos(t)
	};
	mat2 Cis = {
		1, 1.2,
		0, 1
	};
	std::vector<vec2> Q = (Cis*Rot)*P;

	LineStrip L{P.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	render2d(Q, L, blue, G);
	G.savePNG("output.png");
}
