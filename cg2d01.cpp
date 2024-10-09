#include "rasterization.h"
#include "Image.h"

int main(){
	ImageRGB Img(400, 400);
	Img.fill(white);

	for(Pixel p : simple({20, 300}, {200, 300}))
		Img(p.x, p.y) = red;

	for(Pixel p : simple({20, 20}, {80, 334}))
		Img(p.x, p.y) = blue;

	Img.savePNG("output.png");
}
