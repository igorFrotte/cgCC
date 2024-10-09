#include "Render2D_v1.h"

int main(){
	vec2 P[] = {
		{193, 66},  {259, 139},
		{340, 54},  {176, 220},
		{336, 212}, {261, 311}
	};
	unsigned int indices[] = {
		0, 3, 1,   1, 3, 4, 
		1, 4, 2,   3, 5, 4
	};
	TriLines<Elements<Triangles>> T{indices};

	ImageRGB G(500, 400);
	G.fill(white);
	render2d(P, T, blue, G);
	G.savePNG("output.png");
}



