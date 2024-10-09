#include "Render2D_v1.h"
#include "bezier.h"

int main(){
	std::vector<vec2> P = loadCurve(
		//"borboleta.txt"
		"curvaC.txt"
	);
	std::vector<vec2> Q = sample_bezier_spline<3>(P, 50);

	LineStrip LP{P.size()};
	LineStrip LQ{Q.size()};
	
	ImageRGB G(400, 400);
	G.fill(white);
	//render2d(P, LP, blue, G);
	render2d(Q, LQ, red, G);
	G.savePNG("output.png");
}
