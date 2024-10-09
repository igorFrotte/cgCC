#include "transforms.h"

int main(){

   mat4 Model = translate({0.5, 1, 2})*
                rotate_z(0.31)*
                scale(1.5, 1.2, 1.7);
   mat4 View = lookAt({2, 5, 2}, {0, 0, 0}, {0, 1, 0});

   vec4 Q = {5, 2, 1, 1};
  
   print("No referencial do objeto", Q);
   print("No referencial do mundo", Model*Q);
   print("No referencial do observador", View*Model*Q);
}

