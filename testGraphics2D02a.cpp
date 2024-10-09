#include "Render2D_v1.h"

int main(){
	vec2 P[] = {
		{ 60, 105},
		{229, 114},
		{145, 270},
		{364, 208},
		{283, 333},
		{471, 298}
	};

	Triangles T{6};
//	TriangleStrip T{6};

	ImageRGB G(500, 400);
	G.fill(white);
	render2d(P, T, blue, G);
	G.savePNG("output.png");
}


