#include "Render2D.h"
#include "bezier.h"

int main(){
	ClipRectangle R = {100, 200, 500, 400};

	Vec2Col P[] = {
		{{26, 168}, red},
		{{176, 245}, green},
		{{140, 122}, blue},
		{{340, 145}, red},
		{{576, 451}, yellow},
		{{300, 300}, cyan},
		{{124, 357}, green},
		{{154, 457}, black}
	};

	LineStrip L{8};

	vec2 RP[] = {
		{R.x0, R.y0},  {R.x1, R.y0}, 
		{R.x1, R.y1},  {R.x0, R.y1}, 
	};
	LineLoop LR{4};

	ImageRGB G(800, 600);
	G.fill(white);

	render2d(RP, LR, blue, G);
	
	Render2dPipeline pipeline{G};
	//for(Line<Vec2Col> line: assemble(L, P))
	for(Line<Vec2Col> line: clip(assemble(L, P), R))
		pipeline.draw(line);

	G.savePNG("output.png");
}
