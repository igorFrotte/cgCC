#include "Render2D_v1.h"
#include "bezier.h"

int main(){
	vec2 P[] = {
		{11, 218},
		{184, 10},
		{54, 344},
		{315, 369},
		{228, 281},
		{349, 180},
	};
	std::vector<vec2> Q = sample_bezier<5>(P, 100);

	LineStrip LP{6};
	LineStrip LQ{Q.size()};
	
	ImageRGB G(800, 600);
	G.fill(white);
	render2d(P, LP, red, G);
	render2d(Q, LQ, blue, G);
	G.savePNG("output.png");
}
