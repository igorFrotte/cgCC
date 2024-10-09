#include "Image.h"

// Sobrepoe uma cor RGBA sobre um fundo RGB
void over(RGB& Bg, RGBA C){
	vec4 vc = toVec(C);
	float alpha = vc[3];
	vec3 col = lerp(alpha, toVec(Bg), toVec3(vc));
	Bg = toColor(col);
}

int main(){
	ImageRGB img{"background.png"};
	ImageRGBA boy{"flatboy.png"};
	int x = 280, y = 108;
	for(int j = 0; j < boy.height(); j++)
		for(int i = 0; i < boy.width(); i++)
			over( img(x+i, y+j), boy(i, j) );            

	img.savePNG("output.png");
}
