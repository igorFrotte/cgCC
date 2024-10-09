#include "Render2D_v1.h"
#include "bezier.h"

int main(){
	vec2 P[] = {
		{107, 278},
		{166, 83},
		{567, 29},
		{674, 359}
	};
	LineStrip LP{4};

	std::vector<vec2> Q = sample_bezier<3>(P, 50);
	LineStrip LQ{Q.size()};
	
	ImageRGB G(800, 400);
	G.fill(white);
	render2d(P, LP, red, G);
	render2d(Q, LQ, blue, G);
	G.savePNG("output.png");
}
