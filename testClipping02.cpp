#include "Render2D.h"
#include "bezier.h"
#include "polygon_triangulation.h"
int main(){
	ClipRectangle R = {100, 200, 500, 400};

	std::vector<vec2> P = {
		{26, 168},
		{176, 245},
		{140, 122},
		{340, 145},
		{576, 451},
		{300, 300},
		{124, 357},
		{154, 457},
	};

	P = clip(P, R);

	LineLoop L{P.size()};

	std::vector<vec2> RP = {
		{R.x0, R.y0},  {R.x1, R.y0}, 
		{R.x1, R.y1},  {R.x0, R.y1}, 
	};
	LineLoop LR{4};

	ImageRGB G(600, 600);
	G.fill(white);
	render2d(RP, LR, 0xdcdcdc_rgb, G);

	std::vector<unsigned int> indices = triangulate_polygon(P);
	Elements<Triangles> T{indices};
	render2d(P, T, blue, G);
	
	G.savePNG("output.png");
}
