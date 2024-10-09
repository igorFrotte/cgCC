#include "Image.h"

void checkerboard(){
	ImageRGB img(600, 600);
	for (int y = 0; y < 600; y++)
		for (int x = 0; x < 600; x++){
			int m = x * 8 / 600;
			int n = y * 8 / 600;
			img(x, y) = (m + n) % 2 == 0 ? black : white;
		}

	img.savePNG("checkerboard.png");
}

void target(){
	ImageRGB img(600, 600);
	vec2 center = {300, 300};
	for (int y = 0; y < 600; y++)
		for (int x = 0; x < 600; x++){
			vec2 p = {(float)x, (float)y};
			float d = norm(p - center);
			int n = d * 12 / 600;
			img(x, y) = n % 2 == 0 ? black : white;
		}

	img.savePNG("target.png");
}

int main(){
	checkerboard();
	target();
}