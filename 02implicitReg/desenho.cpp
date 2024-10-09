#include "../Image.h"

float G(float x, float y, float cx, float cy, float s){
	float dx = x - cx;
	float dy = y - cy;
	return 100 * exp(-(dx * dx + dy * dy)/(s * s));
}

float F(float x, float y){
	return 50
		- G(x, y, 300, 400, 85)
		- G(x, y, 300, 550, 50)
		- G(x, y, 300, 250, 90)
		- G(x, y, 270, 400, 30)
		- G(x, y, 280, 400, 50)
		- G(x, y, 165, 400, 28)
		- G(x, y, 115, 400, 25)
		- G(x, y, 65, 400, 20)
		- G(x, y, 330, 400, 30)
		- G(x, y, 320, 400, 50)
		- G(x, y, 435, 400, 28)
		- G(x, y, 485, 400, 25)
		- G(x, y, 535, 400, 20)
		- G(x, y, 250, 175, 40)
		- G(x, y, 350, 175, 40)
		- G(x, y, 250, 100, 35)
		- G(x, y, 350, 100, 35)
		- G(x, y, 350, 35, 30)
		- G(x, y, 250, 35, 30);
}

int main(){
	ImageRGB img(600, 600);
	img.fill(white);
	for(int x = 0; x < img.width(); x++)
		for(int y = 0; y < img.height(); y++)
			if(F(x, y) <= 0)
				img(x, y) = green;

	img.savePNG("out.png");
}
