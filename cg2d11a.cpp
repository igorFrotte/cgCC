#include "Render2D.h"
#include "bezier.h"

int main(){
	std::vector<vec2> CP = loadCurve("borboleta.txt");
	std::vector<vec2> P = sample_bezier_spline<3>(CP, 30);

	LineStrip L{P.size()};

	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, L, red, G);
	G.savePNG("output.png");
}
