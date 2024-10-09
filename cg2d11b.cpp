#include "Render2D.h"
#include "bezier.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	vec2 v = {220, 150};

	std::vector<vec2> Q(P.size());
	for(unsigned int i = 0; i < P.size(); i++)
		Q[i] = P[i] + v;
	
	LineStrip L{P.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	render2d(Q, L, blue, G);
	G.savePNG("output.png");
}
