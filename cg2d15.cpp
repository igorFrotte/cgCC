#include "Render2D.h"

int main(){
	std::vector<Vec2Col> P = {
		{{36, 116}, red}, {{108, 90}, green}, {{103, 164}, blue},
		{{-34, -37}, red}, {{32, 78}, green}, {{127, 26}, blue},
		{{145, 43}, red}, {{278, 15}, green}, {{125, -82}, blue}, 
		{{256, 172}, red}, {{271, 69}, green}, {{149, 118}, blue}, 
		{{185, 213}, red}, {{247, 254}, green}, {{229, 188}, blue},
	};

	Triangles T{P.size()};

	ImageRGB G(200, 200);
	G.fill(white);
	render2d(P, T, G);
	G.savePNG("output.png");
}
