#include "Render2D.h"
#include "bezier.h"
#include "matrix.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	mat2 M = {
		-1.0, 0.0,
		 0.0, 1.0
	};	
	vec2 v = {800, 0};

	int n = P.size();
	std::vector<vec2> Q(n);
	for(int i = 0; i < n; i++)
		Q[i] = M*P[i] + v;

	LineStrip L{P.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	render2d(Q, L, blue, G);
	G.savePNG("output.png");
}
