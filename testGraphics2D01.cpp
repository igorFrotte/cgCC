#include "Render2D_v1.h"

int main(){
	vec2 P[] = {
		{20, 20},
		{80, 60},
		{220, 30},
		{350, 90},
		{70, 300},
		{320, 150}
	};

	unsigned int indices[] = {0, 4, 5, 3, 2, 1};

	//Lines L{6};
	//LineStrip L{6};
	//LineLoop L{6};
	Elements<Lines> L{indices};
	//Elements<LineStrip> L{indices};
	//Elements<LineLoop> L{indices};

	ImageRGB G(500, 400);
	G.fill(white);
	render2d(P, L, red, G);
	G.savePNG("output.png");
}


