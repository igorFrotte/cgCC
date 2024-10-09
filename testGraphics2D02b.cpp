#include "Render2D_v1.h"

int main(){
	vec2 P[] = {
		{222, 139},
		{80, 59},
		{117, 279},
		{245, 313},
		{388, 192},
		{354, 32}
	};

	TriangleFan T{6};

	ImageRGB G(500, 400);
	G.fill(white);
	render2d(P, T, blue, G);
	G.savePNG("output.png");
}


