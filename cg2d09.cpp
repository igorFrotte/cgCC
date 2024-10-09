#include "Render2D_v1.h"
#include "bezier.h"

int main(){
	std::vector<vec2> P = {
		{107, 278},
		{166, 83},
		{567, 29},
		{674, 359},
		{500, 581},
		{325, 418},
		{485, 380},
	};
	std::vector<vec2> Q = sample_bezier_spline<2>(P, 50);
	
	LineStrip LP{P.size()};
	LineStrip LQ{Q.size()};

	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, LP, blue, G);
	render2d(Q, LQ, red, G);
	G.savePNG("output.png");
}
