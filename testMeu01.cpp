#include "Render2D.h"
int main(){
   Vec2Col P[] = {
    {{ 60, 105}, red},
    {{229, 114}, green},
    {{145, 270}, blue},
    {{364, 208}, yellow},
    {{283, 333}, cyan},
    {{471, 298}, orange},
  };

  TriangleStrip T{6};

  ImageRGB G(500, 400);
  G.fill(white);
  render2d(P, T, G);
  G.savePNG("output.png");
}
