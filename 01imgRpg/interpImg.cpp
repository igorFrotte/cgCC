#include "../Image.h"

// Sobrepoe uma cor RGBA sobre um fundo RGB
void over(RGB& Bg, RGBA C){
	vec4 vc = toVec(C);
	float alpha = vc[3];
	vec3 col = lerp(alpha, toVec(Bg), toVec3(vc));
	Bg = toColor(col);
}

ImageRGB imgCompose (ImageRGB bg, ImageRGBA img, int x, int y){
    for(int j=0; j<img.height(); j++){
        for(int i=0; i<img.width(); i++){
            over(bg(x+i, y+j), img(i, j));
        }
    }
    return bg;
}

ImageRGB bgCreate(){
    ImageRGB bg{"img/BG.png"};

    ImageRGBA tile1{"img/Tile4.png"};
    ImageRGBA tile2{"img/Tile5.png"};
    ImageRGBA tile3{"img/Tile6.png"};
    ImageRGBA tile4{"img/Tile1.png"};
    ImageRGBA tile5{"img/Tile2.png"};
    ImageRGBA tile6{"img/Tile3.png"};
    ImageRGBA tile7{"img/Tile14.png"};
    ImageRGBA tile8{"img/Tile15.png"};
    ImageRGBA tile9{"img/Tile16.png"};

    ImageRGBA bone1{"img/Bone1.png"};
    ImageRGBA bone2{"img/Bone2.png"};
    ImageRGBA bone3{"img/Bone3.png"};
    ImageRGBA arrow{"img/ArrowSign.png"};
    ImageRGBA tree{"img/Tree.png"};
    ImageRGBA tomb1{"img/TombStone1.png"};
    ImageRGBA tomb2{"img/TombStone2.png"};
    ImageRGBA skeleton{"img/Skeleton.png"};
	
    int x=150, y=0;
    bg = imgCompose(bg, tile1, x, y);

    x+=tile1.width();
    bg = imgCompose(bg, tile2, x, y);

    x+=tile2.width();
    bg = imgCompose(bg, tile2, x, y);
    bg = imgCompose(bg, bone3, x, y);

    x+=tile2.width();
    bg = imgCompose(bg, tile3, x, y);

    x=150;
    y=tile1.height();
    bg = imgCompose(bg, tile1, x, y);

    x+=tile1.width();
    bg = imgCompose(bg, tile2, x, y);
    bg = imgCompose(bg, bone1, x, y);

    x+=tile2.width();
    bg = imgCompose(bg, tile2, x, y);

    x+=tile2.width();
    bg = imgCompose(bg, tile3, x, y);

    x=150;
    y=tile1.height() * 2;
    bg = imgCompose(bg, tile4, x, y);
    bg = imgCompose(bg, tomb2, x + 70, y + tile4.height());

    x+=tile4.width();
    bg = imgCompose(bg, tile5, x, y);

    x+=tile5.width();
    bg = imgCompose(bg, tile5, x, y);
    bg = imgCompose(bg, bone2, x, y);

    x+=tile5.width();
    bg = imgCompose(bg, tile6, x, y);
    bg = imgCompose(bg, arrow, x, y + tile6.height());
    
    x+=100;
    y+=300;
    bg = imgCompose(bg, tile7, x, y);

    x+=tile7.width();
    bg = imgCompose(bg, tile8, x, y);
    bg = imgCompose(bg, tree, x + 80, y + tile8.height());

    x+=tile8.width();
    bg = imgCompose(bg, tile8, x, y);

    x+=tile8.width();
    bg = imgCompose(bg, tile8, x, y);

    x+=tile8.width();
    bg = imgCompose(bg, tile9, x, y);

    y-=400;
    bg = imgCompose(bg, tile7, x, y);

    x+=tile7.width();
    bg = imgCompose(bg, tile8, x, y);
    bg = imgCompose(bg, tomb1, x, y + tile8.height());

    x+=tile8.width();
    bg = imgCompose(bg, tile8, x, y);
    bg = imgCompose(bg, skeleton, x, y + tile8.height());

    x+=tile8.width();
    bg = imgCompose(bg, tile8, x, y);

    x+=tile8.width();
    bg = imgCompose(bg, tile9, x, y);

    bg.savePNG("background.png");
    return bg;
}

int main(){

    ImageRGB bg = bgCreate();

    ImageRGBA char1{"img/girl.png"};
    ImageRGBA char2{"img/dino.png"};
    ImageRGBA char3{"img/male.png"};

    int x=600, y=380;
    bg = imgCompose(bg, char1, x, y); 

    x=250; 
    y=135;
    bg = imgCompose(bg, char2, x, y);   

    x=1400; 
    y=160;
    bg = imgCompose(bg, char3, x, y);

	bg.savePNG("out.png");
}
