#include "Image.h"

int main(){
    ImageRGB I(40, 40);
    I.fill(white);

    I(10, 20) = blue;
    I(10, 30) = red;
    I(20, 30) = green;

    I.savePNG("3points.png");
}