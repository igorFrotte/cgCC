#include "Quaternion.h"

Quaternion quaternion_rotation(vec3 n, float theta){
  return {cosf(theta/2), sinf(theta/2)*normalize(n)};
}

int main(){
  vec3 n = normalize(vec3{2, 3, 5});
  Quaternion q = quaternion_rotation(n, 0.25);
  float theta = 2*acos(q.a);
  vec3 n1 = normalize(q.v);
  printf("theta = %f\n", theta);
  print("n", n);
  print("n", n1);
}


