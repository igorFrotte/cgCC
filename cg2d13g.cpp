#include "Render2D.h"
#include "bezier.h"
#include "matrix.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	vec2 v = {400, 300};
	mat3 T = {
		1.0, 0.0, v[0],
		0.0, 1.0, v[1],
		0.0, 0.0, 1.0
	};
	
	mat3 Ti = {
		1.0, 0.0, -v[0],
		0.0, 1.0, -v[1],
		0.0, 0.0, 1.0
	};
	
	float t = 1.5;
	mat3 R = {
		cos(t), -sin(t), 0.0,
		sin(t),  cos(t), 0.0,
		   0.0,     0.0, 1.0
	};

	LineStrip L{P.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	render2d((T*R*Ti)*P, L, blue, G);
	G.savePNG("output.png");
}
